#include "AddRandomDecoratorToCompositeMutator.h"

#include "EvolvingBehavior.h"

#include "BTMappedDecoratorGene.h"
#include "BTChromosome.h"
#include "BTChromosomeUtils.h"
#include "RandomGen.h"
#include "AddMappedDecoratorToCompositeMutator.h"
#include "ServiceInjector.h"
#include "AddGeneratedDecoratorToCompositeMutator.h"

UAddRandomDecoratorToCompositeMutator::UAddRandomDecoratorToCompositeMutator() : Super()
{
    if ( nullptr == mappedMutator )
    {
        mappedMutator = CreateDefaultSubobject<UAddMappedDecoratorToCompositeMutator>(FName("MappedMutator"));
    }
    
    if ( nullptr == generatedMutator )
    {
        generatedMutator = CreateDefaultSubobject<UAddGeneratedDecoratorToCompositeMutator>(FName("GeneratedMutator"));
    }
}

void UAddRandomDecoratorToCompositeMutator::Init(UServiceInjector* injector )
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
UAddRandomDecoratorToCompositeMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
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
