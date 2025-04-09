#include "PerturbPropertyInDecoratorMutator.h"

#include "EvolvingBehavior.h"

#include "BTChromosomeUtils.h"
#include "BTGeneratedDecoratorGene.h"
#include "ServiceInjector.h"

void UPerturbPropertyInDecoratorMutator::Init( UServiceInjector* injector )
{
    UMutator::Init(injector);
}

UBTChromosome*
UPerturbPropertyInDecoratorMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    UBTCompositeNodeGene* parent = nullptr;
    UBTGeneratedDecoratorGene* decoratorNode = nullptr;
	uint16 childIdx;
    URandomGen* rand = serviceInjector->GetService<URandomGen>();

    BTChromosomeUtils::FindRandomNode<UBTGeneratedDecoratorGene>( chromosome, rand, parent, childIdx, decoratorNode );

    if ( nullptr == decoratorNode )
    {
        // No appropriate node found, stop.
        return chromosome;
    }

    decoratorNode->MutateRandomProperty(ScaleOfPerturbation);

    return chromosome;
}
