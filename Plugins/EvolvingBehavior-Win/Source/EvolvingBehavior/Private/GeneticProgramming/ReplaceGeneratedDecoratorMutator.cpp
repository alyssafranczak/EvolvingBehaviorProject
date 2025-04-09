#include "ReplaceGeneratedDecoratorMutator.h"

#include "EvolvingBehavior.h"

#include "BTCompositeNodeGene.h"
#include "BTChildContainerGene.h"
#include "BTChromosomeUtils.h"
#include "BTGeneratedDecoratorGene.h"
#include "BTChromosome.h"
#include "BTDecoratorGene.h"
#include "BTGeneratedTemplateLibrary.h"
#include "BTNodeGene.h"

void UReplaceGeneratedDecoratorMutator::Init( UServiceInjector* injector )
{
    UMutator::Init(injector);
    genLibrary = serviceInjector->GetService<UBTGeneratedTemplateLibrary>();
}

UBTChromosome*
UReplaceGeneratedDecoratorMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    UBTCompositeNodeGene* parent = nullptr;
    UBTNodeGene* oldNode = nullptr;
    uint16 childIdx;

    URandomGen* rand = serviceInjector->GetService<URandomGen>();

    // Sets oldNode to a generated decorator node from the current behavior tree. Parent is the composite node who owns it, and childIdx is the index of the container holding the found node within the parent
    BTChromosomeUtils::FindRandomNodeOfType(
        chromosome, rand, UBTGeneratedDecoratorGene::StaticClass(), parent, childIdx, oldNode );

    if( nullptr == oldNode )
	{
		// We didn't find any decorators - probably there are none in the Chromosome. This is ok.
		return chromosome;
	}

    UBTDecoratorGene* oldDecorator = Cast<UBTDecoratorGene>( oldNode );
    if( nullptr == oldDecorator )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Error: found a non-decorator to replace? Node: %s" ),
	        *oldNode->ToString() );
	}

    UObject* parentObject = parent;
    if( nullptr == parent )
	{
	    parentObject = chromosome;
	}

    UBTGeneratedDecoratorGene* newNode = genLibrary->GetRandomGeneratedDecorator( rand, parentObject );
    
    if( newNode == nullptr ) {
        return chromosome;
    }
    
    // mutates node so it starts with random values in its range
    newNode->RandomizeAllProperties();

    if (nullptr == newNode)
    {
        // We didn't find a node to add, so just stop.
        return chromosome;
    }

    // If we were not returned a parent, this is a root decorator.
    if( nullptr == parent )
	{
	    int32 oldDecoratorIndex = chromosome->RemoveDecorator( oldDecorator );
	    if( oldDecoratorIndex < 0 )
		{
		    UE_LOG( LogEvolvingBehaviorError, Error,
		        TEXT( "Error: could not remove decorator - not found in parent: %s " ),
		        *oldDecorator->ToString() );
		}
	    chromosome->InsertDecorator( newNode, oldDecoratorIndex );
	    return chromosome;
	}

    UBTChildContainerGene* childContainer = parent->GetChild( childIdx );

    int32 oldDecoratorIndex = childContainer->RemoveDecorator( oldDecorator );
    if( oldDecoratorIndex < 0 )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error,
	        TEXT( "Error: could not remove decorator - not found in parent: %s " ), *oldDecorator->ToString() );
	}
    childContainer->InsertDecorator( newNode, oldDecoratorIndex );

    return chromosome;
}
