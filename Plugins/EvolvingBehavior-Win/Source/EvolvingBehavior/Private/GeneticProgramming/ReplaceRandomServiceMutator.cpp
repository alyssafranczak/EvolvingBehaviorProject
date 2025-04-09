#include "ReplaceRandomServiceMutator.h"

#include "EvolvingBehavior.h"

#include "BTMappedDecoratorGene.h"
#include "BTChromosome.h"
#include "BTChromosomeUtils.h"
#include "RandomGen.h"
#include "ReplaceMappedServiceMutator.h"
#include "ReplaceGeneratedServiceMutator.h"

UReplaceRandomServiceMutator::UReplaceRandomServiceMutator()
{
    if ( nullptr == mappedMutator )
    {
        mappedMutator = CreateDefaultSubobject<UReplaceMappedServiceMutator>(FName("MappedMutator"));
    }

    if ( nullptr == generatedMutator )
    {
        generatedMutator = CreateDefaultSubobject<UReplaceGeneratedServiceMutator>(FName("GeneratedMutator"));
    }
}

void UReplaceRandomServiceMutator::Init( UServiceInjector* injector )
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
UReplaceRandomServiceMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
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
