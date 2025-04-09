#include "RemoveMappedCompositeMutator.h"

#include "EvolvingBehavior.h"

#include "BTChromosomeUtils.h"
#include "BTMappedCompositeGene.h"
#include "BTChildContainerGene.h"
#include "ServiceInjector.h"

UBTChromosome*
URemoveMappedCompositeMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    UBTCompositeNodeGene* parent = nullptr;
    UBTNodeGene* compositeNode = nullptr;
	uint16 childIdx;

    URandomGen* rand = serviceInjector->GetService<URandomGen>();

    // Sets compositeNode to a random composite node from the current behavior tree. Parent is the composite node who owns it, and childIdx is the index of the container holding the found node within the parent
    BTChromosomeUtils::FindRandomNodeOfType( chromosome, rand, UBTMappedCompositeGene::StaticClass(), parent, childIdx, compositeNode );

    // parent exists (not the root)
    if( nullptr != parent )
	{
        parent->RemoveChild( parent->GetChild( childIdx ) );
	    CopyData( Cast<UBTCompositeNodeGene>( compositeNode ), parent );
	}
    // can't remove it if it is the root
    else
	{
        return chromosome;
	}
    return chromosome;
}

// copies the children from the first given node to the second
void URemoveMappedCompositeMutator::CopyData( UBTCompositeNodeGene* nodeToRemove, UBTCompositeNodeGene* parentNode )
{
    for( int i = 0; i < nodeToRemove->GetNumChildren(); ++i )
	{
	    UBTChildContainerGene* child = nodeToRemove->GetChild( i );
        // Re-parent the child to the new node
		child->Rename(nullptr, parentNode);
	    parentNode->AddChild( child );
	}
}
