#include "AddRandomTaskMutator.h"

#include "EvolvingBehavior.h"

#include "BTMappedDecoratorGene.h"
#include "BTChromosome.h"
#include "BTChromosomeUtils.h"
#include "RandomGen.h"
#include "AddMappedTaskMutator.h"
#include "ServiceInjector.h"
#include "AddGeneratedTaskMutator.h"

UAddRandomTaskMutator::UAddRandomTaskMutator() : Super()
{
    if ( nullptr == mappedMutator )
    {
        mappedMutator = CreateDefaultSubobject<UAddMappedTaskMutator>(FName("MappedMutator"));
    }
    
    if ( nullptr == generatedMutator )
    {
        generatedMutator = CreateDefaultSubobject<UAddGeneratedTaskMutator>(FName("GeneratedMutator"));
    }
}

void UAddRandomTaskMutator::Init( UServiceInjector* injector )
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
UAddRandomTaskMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
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
