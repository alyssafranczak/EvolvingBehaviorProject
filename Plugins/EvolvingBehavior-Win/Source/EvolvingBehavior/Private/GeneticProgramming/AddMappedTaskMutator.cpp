#include "AddMappedTaskMutator.h"

#include "EvolvingBehavior.h"

#include "BTChildContainerGene.h"
#include "BTChromosomeUtils.h"
#include "BTCompositeNodeGene.h"
#include "ServiceInjector.h"
#include "BTMappedTaskGene.h"

void UAddMappedTaskMutator::Init(UServiceInjector* injector )
{
    UMutator::Init(injector);
    mapper = injector->GetService<UNodeMapper>();
}

UBTChromosome*
UAddMappedTaskMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    UBTCompositeNodeGene* parent = nullptr;
    UBTNodeGene* compositeNode = nullptr;
	uint16 childIdx;

    URandomGen* rand = serviceInjector->GetService<URandomGen>();

    // Sets compositeNode to a random composite node from the current behavior tree. Parent is the composite node who owns it, and childIdx is the index of the container holding the found node within the parent
    BTChromosomeUtils::FindRandomNodeOfType( chromosome, rand, UBTCompositeNodeGene::StaticClass(), parent, childIdx, compositeNode );
    
    if( nullptr == parent )
    {
        compositeNode = chromosome->GetRoot();
    }

    FNodeMapID newNodeID = mapper->GetRandomTaskID( rand );

    if (newNodeID == FNodeMapID::INVALID_ID)
    {
        // We didn't find a task to add, so just stop.
        return chromosome;
    }
    
    UBTChildContainerGene* childContainer = NewObject<UBTChildContainerGene>(compositeNode, UBTChildContainerGene::StaticClass());

    UBTMappedTaskGene* randomNewNode
        = NewObject<UBTMappedTaskGene>( childContainer, UBTMappedTaskGene::StaticClass() );
    randomNewNode->Init( serviceInjector, newNodeID );

    childContainer->SetChild( randomNewNode );

    Cast<UBTCompositeNodeGene>(compositeNode)->AddChild( childContainer );
    
    return chromosome;
}
