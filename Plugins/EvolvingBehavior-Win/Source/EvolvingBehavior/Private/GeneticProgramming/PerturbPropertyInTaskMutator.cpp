#include "PerturbPropertyInTaskMutator.h"

#include "EvolvingBehavior.h"

#include "BTChromosomeUtils.h"
#include "BTGeneratedTaskGene.h"
#include "ServiceInjector.h"

void UPerturbPropertyInTaskMutator::Init( UServiceInjector* injector )
{
    UMutator::Init(injector);
}

UBTChromosome*
UPerturbPropertyInTaskMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    UBTCompositeNodeGene* parent = nullptr;
    UBTGeneratedTaskGene* taskNode = nullptr;
	uint16 childIdx;
    URandomGen* rand = serviceInjector->GetService<URandomGen>();

    BTChromosomeUtils::FindRandomNode<UBTGeneratedTaskGene>( chromosome, rand, parent, childIdx, taskNode );

    if ( nullptr == taskNode )
    {
        // No appropriate node found, stop.
        return chromosome;
    }

    taskNode->MutateRandomProperty(ScaleOfPerturbation);

    return chromosome;
}
