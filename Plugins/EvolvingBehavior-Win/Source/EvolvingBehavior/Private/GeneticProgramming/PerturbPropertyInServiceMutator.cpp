#include "PerturbPropertyInServiceMutator.h"

#include "EvolvingBehavior.h"

#include "BTChromosomeUtils.h"
#include "BTGeneratedServiceGene.h"
#include "ServiceInjector.h"

void UPerturbPropertyInServiceMutator::Init( UServiceInjector* injector )
{
    UMutator::Init(injector);
}

UBTChromosome*
UPerturbPropertyInServiceMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    UBTCompositeNodeGene* parent = nullptr;
    UBTGeneratedServiceGene* serviceNode = nullptr;
	uint16 childIdx;
    URandomGen* rand = serviceInjector->GetService<URandomGen>();

    BTChromosomeUtils::FindRandomNode<UBTGeneratedServiceGene>( chromosome, rand, parent, childIdx, serviceNode );

    if ( nullptr == serviceNode )
    {
        // No appropriate node found, stop.
        return chromosome;
    }

    serviceNode->MutateRandomProperty(ScaleOfPerturbation);

    return chromosome;
}
