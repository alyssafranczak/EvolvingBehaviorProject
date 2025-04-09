#include "AddGeneratedDecoratorToTaskMutator.h"

#include "EvolvingBehavior.h"

#include "BTGeneratedDecoratorGene.h"
#include "BTChildContainerGene.h"
#include "BTChromosomeUtils.h"
#include "BTChromosome.h"
#include "ServiceInjector.h"
#include "BTGeneratedTemplateLibrary.h"

void UAddGeneratedDecoratorToTaskMutator::Init( UServiceInjector* injector )
{
    UMutator::Init(injector);
    genLibrary = serviceInjector->GetService<UBTGeneratedTemplateLibrary>();
}

UBTChromosome*
UAddGeneratedDecoratorToTaskMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    UBTCompositeNodeGene* parent = nullptr;
    UBTNodeGene* taskNode = nullptr;
	uint16 childIdx;
    
    URandomGen* rand = serviceInjector->GetService<URandomGen>();

    // Sets taskNode to a random composite node from the current behavior tree. Parent is the composite node who owns it, and childIdx is the index of the container holding the found node within the parent
    BTChromosomeUtils::FindRandomNodeOfType( chromosome, rand, UBTTaskNodeGene::StaticClass(), parent, childIdx, taskNode );

    UObject* parentObject = nullptr;
    
    if( nullptr != parent )
	{
        parentObject = parent->GetChild( childIdx );
	}
    else
    {
        parentObject = chromosome;
    }

    UBTGeneratedDecoratorGene* newNode = genLibrary->GetRandomGeneratedDecorator( rand, parentObject );
    
    if( newNode == nullptr ) {
        UE_LOG( LogEvolvingBehavior, Error, TEXT( "No Generated Decorator was added (none in library)" ) );
        return chromosome;
    }
    
    // mutates node so it starts with random values in its range
    newNode->RandomizeAllProperties();

    if (nullptr == newNode)
    {
        // We didn't find a node to add, so just stop.
        return chromosome;
    }

    // if the parent is not empty (this isnt the root node)
    if( nullptr != parent )
	{
        UBTChildContainerGene* toAddTo = parent->GetChild( childIdx );
        toAddTo->AddDecorator( newNode );
	}
    else
	{
        chromosome->AddDecorator( newNode );
	}

    return chromosome;
}
