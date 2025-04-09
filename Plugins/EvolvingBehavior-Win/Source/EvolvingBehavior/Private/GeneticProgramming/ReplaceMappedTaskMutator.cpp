#include "ReplaceMappedTaskMutator.h"

#include "EvolvingBehavior.h"

#include "BTCompositeNodeGene.h"
#include "BTNodeGene.h"
#include "BTChromosomeUtils.h"
#include "NodeMapper.h"
#include "BTMappedTaskGene.h"
#include "BTChildContainerGene.h"

void UReplaceMappedTaskMutator::Init( UServiceInjector* injector )
{
    UMutator::Init(injector);
    mapper = serviceInjector->GetService<UNodeMapper>();
}

UBTChromosome*
UReplaceMappedTaskMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    UBTCompositeNodeGene* parent = nullptr;
	UBTNodeGene* oldNode = nullptr;
    uint16 childIdx;

    URandomGen* rand = serviceInjector->GetService<URandomGen>();
    
    // Sets oldNode to a random mapped task node from the current behavior tree. Parent is the composite node who owns it, and childIdx is the index of the container holding the found node within the parent
    BTChromosomeUtils::FindRandomNodeOfType( chromosome, rand, UBTMappedTaskGene::StaticClass(), parent, childIdx, oldNode );

    if( nullptr == parent )
	{
	    UE_LOG( LogEvolvingBehavior, Display, TEXT( "Error: no mapped task found to replace in chromosome: %s" ),
	        *chromosome->ToString() );
	    return chromosome;
	}

    FNodeMapID newNodeID = mapper->GetRandomTaskID( rand );

    if (newNodeID == FNodeMapID::INVALID_ID)
    {
        // We didn't find a node to add, so just stop.
        return chromosome;
    }
    
    UBTChildContainerGene* childContainer = parent->GetChild( childIdx );

    UBTMappedTaskGene* randomNewNode
        = NewObject<UBTMappedTaskGene>( childContainer, UBTMappedTaskGene::StaticClass() );
    randomNewNode->Init( serviceInjector, newNodeID );

    childContainer->ReplaceChild( randomNewNode );
	
	return chromosome;
}
