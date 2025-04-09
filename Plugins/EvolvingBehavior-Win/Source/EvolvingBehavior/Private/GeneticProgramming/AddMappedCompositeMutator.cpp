#include "AddMappedCompositeMutator.h"

#include "EvolvingBehavior.h"

#include "BTChildContainerGene.h"
#include "BTCompositeNodeGene.h"
#include "BTChromosomeUtils.h"
#include "ServiceInjector.h"
#include "BTMappedCompositeGene.h"

void UAddMappedCompositeMutator::Init( UServiceInjector* injector )
{
    UMutator::Init(injector);
    mapper = serviceInjector->GetService<UNodeMapper>();
}

UBTChromosome*
UAddMappedCompositeMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    UBTCompositeNodeGene* parent = nullptr;
    UBTNodeGene* foundNode = nullptr;
	uint16 childIdx;

    URandomGen* rand = serviceInjector->GetService<URandomGen>();
    
    BTChromosomeUtils::FindRandomNodeOfType( chromosome, rand, UBTCompositeNodeGene::StaticClass(), parent, childIdx, foundNode );

    UObject* parentObject = nullptr;
    if( nullptr != parent )
    {
        parentObject = parent->GetChild( childIdx );
    }
    else
    {
        parentObject = chromosome;
    }
    
    UBTCompositeNodeGene* compositeNode = Cast<UBTCompositeNodeGene>( foundNode );

    FNodeMapID newNodeID = mapper->GetRandomCompositeID( rand );

    if (newNodeID == FNodeMapID::INVALID_ID)
    {
        // We didn't find a node to add, so just stop.
        return chromosome;
    }
    
    if ( nullptr == compositeNode)
    {
       // Get the old root (to add under the new composite)
        UBTCompositeNodeGene* oldRoot = chromosome->GetRoot();
        
        // gets a randon new composite from the mapper. The parent is the chromosome
        UBTMappedCompositeGene* randomNewNode = NewObject<UBTMappedCompositeGene>( chromosome, UBTMappedCompositeGene::StaticClass() );
        randomNewNode->Init( serviceInjector, newNodeID );
        // sets the new node as the root
        chromosome->SetRoot(randomNewNode);
        // creates a new composite child whos parent is the new node we found
        UBTChildContainerGene* newCompositeChild = NewObject<UBTChildContainerGene>(randomNewNode, UBTChildContainerGene::StaticClass());
        // adds the composite child as the child of the new root node
        randomNewNode->AddChild(newCompositeChild);
        // sets the old roots parent to be the composite child we created
        UBTCompositeNodeGene* oldRootCopy = oldRoot->DuplicateComposite(newCompositeChild);
        // sets the old root to be the child of the new composite child
        newCompositeChild->SetChild( oldRootCopy );
    }
    else
    {
        // creates a composite child whos parent is the composite we chose
        UBTChildContainerGene* newCompositeChild = NewObject<UBTChildContainerGene>(compositeNode, UBTChildContainerGene::StaticClass());
        // gets a randon new composite from the mapper. The parent is the composite child we made
        UBTMappedCompositeGene* randomNewNode = NewObject<UBTMappedCompositeGene>( newCompositeChild, UBTMappedCompositeGene::StaticClass() );
        randomNewNode->Init( serviceInjector, newNodeID );
        // sets the new node as the child of our composite child
        newCompositeChild->SetChild( randomNewNode );
        // adds the new composite child to the chosen node
        compositeNode->AddChild( newCompositeChild );
    }

    return chromosome;
}
