#include "ReplaceMappedCompositeMutator.h"

#include "EvolvingBehavior.h"

#include "BTChildContainerGene.h"
#include "BTCompositeNodeGene.h"
#include "BTMappedCompositeGene.h"
#include "NodeMapID.h"
#include "BTChromosomeUtils.h"

void UReplaceMappedCompositeMutator::Init( UServiceInjector* injector )
{
    UMutator::Init(injector);
    mapper = serviceInjector->GetService<UNodeMapper>();
}

UBTChromosome*
UReplaceMappedCompositeMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    UBTCompositeNodeGene* parent = nullptr;
    UBTNodeGene* oldNode = nullptr;
	uint16 childIdx;

    URandomGen* rand = serviceInjector->GetService<URandomGen>();
    
    // Sets oldNode to a random composite node from the current behavior tree. Parent is the composite node who owns it, and childIdx is the index of the container holding the found node within the parent
    BTChromosomeUtils::FindRandomNodeOfType( chromosome, rand, UBTMappedCompositeGene::StaticClass(), parent, childIdx, oldNode );

    UObject* parentObject = parent;
    if( nullptr == parentObject )
	{
	    parentObject = chromosome;
	}

    FNodeMapID newNodeID = mapper->GetRandomCompositeID( rand );

    if (newNodeID == FNodeMapID::INVALID_ID)
    {
        // We didn't find a node to add, so just stop.
        return chromosome;
    }
    
    if( nullptr != parent )
	{
	    UBTChildContainerGene* toReplace = parent->GetChild( childIdx );
        
        UBTMappedCompositeGene* randomNewNode
        = NewObject<UBTMappedCompositeGene>( toReplace, UBTMappedCompositeGene::StaticClass() );
        randomNewNode->Init( serviceInjector, newNodeID );
        
	    if( toReplace->IsALeaf() )
		{
		    UE_LOG( LogEvolvingBehaviorError, Error,
		        TEXT( "Child is a leaf but should be a composite?! Leaf: %s, Parent: %s" ),
		        *toReplace->GetTaskChild()->ToString(), *parent->ToString() );
		}
	    CopyData( toReplace->GetCompositeChild(), randomNewNode );
	    toReplace->ReplaceChild( randomNewNode );
	}
    else
	{
	    UBTCompositeNodeGene* toReplace = chromosome->GetRoot();
        
        UBTMappedCompositeGene* randomNewNode
        = NewObject<UBTMappedCompositeGene>( parentObject, UBTMappedCompositeGene::StaticClass() );
        randomNewNode->Init( serviceInjector, newNodeID );
        
	    CopyData( toReplace, randomNewNode );
	    chromosome->SetRoot( randomNewNode );
	}

    return chromosome;
}

void UReplaceMappedCompositeMutator::CopyData( UBTCompositeNodeGene* oldNode, UBTCompositeNodeGene* newNode )
{
    for( int i = 0; i < oldNode->GetNumChildren(); ++i )
	{
	    UBTChildContainerGene* child = oldNode->GetChild( i );
		// Re-parent the child to the new node
        UBTChildContainerGene* childCopy = child->Duplicate(newNode);
	    newNode->AddChild( childCopy );
	}
}
