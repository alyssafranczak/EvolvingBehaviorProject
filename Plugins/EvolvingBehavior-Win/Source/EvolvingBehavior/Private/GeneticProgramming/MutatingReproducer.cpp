#include "MutatingReproducer.h"

#include "EvolvingBehavior.h"

#include "BTChromosome.h"
#include "Mutator.h"
#include "NodeMapper.h"
#include "BTGeneratedTaskGene.h"
#include "BTGeneratedDecoratorGene.h"
#include "BTGeneratedServiceGene.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BehaviorTree/BTService.h"
#include "BehaviorTree/BTDecorator.h"
#include "RandomGen.h"
#include "ServiceInjector.h"
#include "BTGeneratedTemplateLibrary.h"
#include "CrossoverMutator.h"

void UMutatingReproducer::Init( TArray<FMutatorInfo> newMutators, UServiceInjector* serviceInjector )
{
    mutators = newMutators;

    Init( serviceInjector );
}

void UMutatingReproducer::Init( UServiceInjector* serviceInjector )
{
    Super::Init(serviceInjector);

    //FIXME this seems messy
    UBTGeneratedTemplateLibrary* generatedLibrary = serviceInjector->GetService<UBTGeneratedTemplateLibrary>();
    
    for( int m = 0; m < mutators.Num(); ++m )
	{
        if( mutators[m].mutator == nullptr ) {
            UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "All mutators must be set, The index: %i was set to 'None' check your Mutating Reproducer settings to remove or set this index to a valid value." ), m );
            mutators.RemoveAt(m);
            m--;
            continue;
        }
        mutators[m].mutator->Init(serviceInjector);
	}
}

TArray<UBTChromosome*> UMutatingReproducer::Reproduce( TArray<UBTChromosome*> parents )
{
    TArray<UBTChromosome*> children = Super::Reproduce( parents );

    URandomGen* rand = ServiceInjector->GetService<URandomGen>();

    for( int i = 0; i < children.Num(); ++i )
	{
	    for( int m = 0; m < mutators.Num(); ++m )
		{
		    if( rand->WithProbability( mutators[m].probability ) )
			{
			    children[i] = mutators[m].mutator->Mutate( children[i], parents, i );
			}
		}
	}

    return children;
}
