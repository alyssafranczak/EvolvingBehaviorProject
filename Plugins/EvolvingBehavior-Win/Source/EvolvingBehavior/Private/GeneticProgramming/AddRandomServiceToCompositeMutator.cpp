#include "AddRandomServiceToCompositeMutator.h"

#include "EvolvingBehavior.h"

#include "BTMappedDecoratorGene.h"
#include "BTChromosome.h"
#include "BTChromosomeUtils.h"
#include "RandomGen.h"
#include "AddMappedServiceToCompositeMutator.h"
#include "ServiceInjector.h"
#include "AddGeneratedServiceToCompositeMutator.h"

UAddRandomServiceToCompositeMutator::UAddRandomServiceToCompositeMutator() : Super()
{
    if ( nullptr == mappedMutator )
    {
        mappedMutator = CreateDefaultSubobject<UAddMappedServiceToCompositeMutator>(FName("MappedMutator"));
    }
    
    if ( nullptr == generatedMutator )
    {
        generatedMutator = CreateDefaultSubobject<UAddGeneratedServiceToCompositeMutator>(FName("GeneratedMutator"));
    }
}

void UAddRandomServiceToCompositeMutator::Init( UServiceInjector* injector )
{
    if (nullptr != mappedMutator )
    {
        mappedMutator->Init(injector);
    }

    if (nullptr != generatedMutator)
    {
        generatedMutator->Init(injector);
    }
    
    UMutator::Init(injector);
}

UBTChromosome*
UAddRandomServiceToCompositeMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    URandomGen* rand = serviceInjector->GetService<URandomGen>();

    if(rand->WithProbability(probabilityOfMapped)) {
        chromosome = mappedMutator->Mutate(chromosome,parents,primaryParent);
    }
    else {
        chromosome = generatedMutator->Mutate(chromosome,parents,primaryParent);
    }
    return chromosome;
}
