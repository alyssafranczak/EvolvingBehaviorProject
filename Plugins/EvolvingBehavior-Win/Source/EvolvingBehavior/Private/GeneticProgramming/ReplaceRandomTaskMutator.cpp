#include "ReplaceRandomTaskMutator.h"

#include "EvolvingBehavior.h"

#include "BTMappedDecoratorGene.h"
#include "BTChromosome.h"
#include "BTChromosomeUtils.h"
#include "RandomGen.h"
#include "ReplaceMappedTaskMutator.h"
#include "ReplaceGeneratedTaskMutator.h"

UReplaceRandomTaskMutator::UReplaceRandomTaskMutator()
{
    if ( nullptr == mappedMutator )
    {
        mappedMutator = CreateDefaultSubobject<UReplaceMappedTaskMutator>(FName("MappedMutator"));
    }

    if ( nullptr == generatedMutator )
    {
        generatedMutator = CreateDefaultSubobject<UReplaceGeneratedTaskMutator>(FName("GeneratedMutator"));
    }
}

void UReplaceRandomTaskMutator::Init( UServiceInjector* injector )
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
UReplaceRandomTaskMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
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
