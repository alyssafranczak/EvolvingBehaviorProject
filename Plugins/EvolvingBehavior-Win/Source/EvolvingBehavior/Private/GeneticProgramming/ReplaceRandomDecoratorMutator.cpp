#include "ReplaceRandomDecoratorMutator.h"

#include "EvolvingBehavior.h"

#include "BTMappedDecoratorGene.h"
#include "BTChromosome.h"
#include "BTChromosomeUtils.h"
#include "RandomGen.h"
#include "ReplaceMappedDecoratorMutator.h"
#include "ReplaceGeneratedDecoratorMutator.h"

UReplaceRandomDecoratorMutator::UReplaceRandomDecoratorMutator()
{
    if ( nullptr == mappedMutator )
    {
        mappedMutator = CreateDefaultSubobject<UReplaceMappedDecoratorMutator>(FName("MappedMutator"));
    }

    if ( nullptr == generatedMutator )
    {
        generatedMutator = CreateDefaultSubobject<UReplaceGeneratedDecoratorMutator>(FName("GeneratedMutator"));
    }
}

void UReplaceRandomDecoratorMutator::Init( UServiceInjector* injector )
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
UReplaceRandomDecoratorMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
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
