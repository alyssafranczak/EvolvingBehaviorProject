#include "RemoveRandomServiceMutator.h"

#include "EvolvingBehavior.h"

#include "BTMappedDecoratorGene.h"
#include "BTChromosome.h"
#include "BTChromosomeUtils.h"
#include "RandomGen.h"
#include "RemoveMappedServiceMutator.h"
#include "RemoveGeneratedServiceMutator.h"

URemoveRandomServiceMutator::URemoveRandomServiceMutator() : Super()
{
    if ( nullptr == mappedMutator )
    {
        mappedMutator = CreateDefaultSubobject<URemoveMappedServiceMutator>(FName("MappedMutator"));
    }
    
    if ( nullptr == generatedMutator )
    {
        generatedMutator = CreateDefaultSubobject<URemoveGeneratedServiceMutator>(FName("GeneratedMutator"));
    }
}

void URemoveRandomServiceMutator::Init( UServiceInjector* injector )
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
URemoveRandomServiceMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
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
