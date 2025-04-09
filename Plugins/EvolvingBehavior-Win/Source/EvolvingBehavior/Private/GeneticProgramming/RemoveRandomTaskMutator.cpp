#include "RemoveRandomTaskMutator.h"

#include "EvolvingBehavior.h"

#include "BTMappedDecoratorGene.h"
#include "BTChromosome.h"
#include "BTChromosomeUtils.h"
#include "RandomGen.h"
#include "RemoveMappedTaskMutator.h"
#include "RemoveGeneratedTaskMutator.h"

URemoveRandomTaskMutator::URemoveRandomTaskMutator() : Super()
{
    if ( nullptr == mappedMutator )
    {
        mappedMutator = CreateDefaultSubobject<URemoveMappedTaskMutator>(FName("MappedMutator"));
    }
    
    if ( nullptr == generatedMutator )
    {
        generatedMutator = CreateDefaultSubobject<URemoveGeneratedTaskMutator>(FName("GeneratedMutator"));
    }
}

void URemoveRandomTaskMutator::Init( UServiceInjector* injector )
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

UBTChromosome* URemoveRandomTaskMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    URandomGen* rand = serviceInjector->GetService<URandomGen>();
    if(rand->WithProbability(probabilityOfMapped))
    {
        chromosome = mappedMutator->Mutate(chromosome,parents,primaryParent);
    }
    else
    {
        chromosome = generatedMutator->Mutate(chromosome,parents,primaryParent);
    }
    return chromosome;
}
