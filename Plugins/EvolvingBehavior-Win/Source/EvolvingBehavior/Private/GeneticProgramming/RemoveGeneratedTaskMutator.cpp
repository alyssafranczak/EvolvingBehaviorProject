#include "RemoveGeneratedTaskMutator.h"

#include "EvolvingBehavior.h"

#include "BTGeneratedTaskGene.h"
#include "BTChildContainerGene.h"
#include "BTChromosomeUtils.h"
#include "ServiceInjector.h"

UBTChromosome*
URemoveGeneratedTaskMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    UBTCompositeNodeGene* parent = nullptr;
    UBTNodeGene* taskNode = nullptr;
	uint16 childIdx;
    
    URandomGen* rand = serviceInjector->GetService<URandomGen>();
    
    // Sets taskNode to a random generated task node from the current behavior tree. Parent is the composite node who owns it, and childIdx is the index of the container holding the found node within the parent
    BTChromosomeUtils::FindRandomNodeOfType( chromosome, rand, UBTGeneratedTaskGene::StaticClass(), parent, childIdx, taskNode );
    
    if( nullptr == taskNode )
    {
        // We didn't find any generated tasks - probably there are none in the Chromosome. This is ok.
        return chromosome;
    }

    // doesn't remove the task if it is the root
    if( nullptr != parent )
	{
        parent->RemoveChild ( parent->GetChild(childIdx) );
	}
    else
    {
        return chromosome;
    }
    return chromosome;
}
