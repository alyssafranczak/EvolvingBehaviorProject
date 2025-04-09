#include "RemoveMappedDecoratorMutator.h"

#include "EvolvingBehavior.h"

#include "BTDecoratorGene.h"
#include "BTMappedDecoratorGene.h"
#include "BTCompositeNodeGene.h"
#include "BTChromosomeUtils.h"
#include "BTChildContainerGene.h"
#include "ServiceInjector.h"
#include "RandomGen.h"

UBTChromosome*
URemoveMappedDecoratorMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    UBTCompositeNodeGene* parent = nullptr;
    UBTNodeGene* decoratorNode = nullptr;
    uint16 childIdx;

    URandomGen* rand = serviceInjector->GetService<URandomGen>();

    // Sets decoratorNode to a random composite node from the current behavior tree. Parent is the composite node who owns it, and childIdx is the index of the container holding the found node within the parent
    BTChromosomeUtils::FindRandomNodeOfType(
        chromosome, rand, UBTMappedDecoratorGene::StaticClass(), parent, childIdx, decoratorNode );

    if( nullptr == decoratorNode )
	{
		// We didn't find any decorators - probably there are none in the Chromosome. This is ok.
		return chromosome;
	}

    // If we were not returned a parent, this is a root decorator.
    if( nullptr == parent )
	{
	    chromosome->RemoveDecorator( Cast<UBTDecoratorGene>( decoratorNode ) );
	    return chromosome;
	}
    else
    {
        UBTChildContainerGene* toRemoveFrom = parent->GetChild( childIdx );
        
        toRemoveFrom->RemoveDecorator( Cast<UBTDecoratorGene>( decoratorNode ) );
        
        return chromosome;
    }
    
}
