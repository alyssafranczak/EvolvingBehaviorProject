#include "RemoveMappedServiceMutator.h"

#include "EvolvingBehavior.h"

#include "BTServiceGene.h"
#include "BTMappedServiceGene.h"
#include "BTCompositeNodeGene.h"
#include "BTChromosomeUtils.h"
#include "BTChildContainerGene.h"
#include "RandomGen.h"
#include "ServiceInjector.h"

UBTChromosome*
URemoveMappedServiceMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    UBTCompositeNodeGene* parent = nullptr;
    UBTNodeGene* serviceNode = nullptr;
    uint16 childIdx;

    URandomGen* rand = serviceInjector->GetService<URandomGen>();

    // Sets serviceNode to a random composite node from the current behavior tree. Parent is the composite node who owns it, and childIdx is the index of the container holding the found node within the parent
    BTChromosomeUtils::FindRandomNodeOfType(
        chromosome, rand, UBTMappedServiceGene::StaticClass(), parent, childIdx, serviceNode );

    if( nullptr == serviceNode )
	{
		// We didn't find any services - probably there are none in the Chromosome. This is ok.
		return chromosome;
	}
    
    else
    {
        parent->RemoveService( Cast<UBTServiceGene>( serviceNode ) );
        
        return chromosome;
    }
    
}
