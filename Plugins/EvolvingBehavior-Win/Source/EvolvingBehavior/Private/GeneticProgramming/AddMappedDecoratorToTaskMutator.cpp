#include "AddMappedDecoratorToTaskMutator.h"

#include "EvolvingBehavior.h"

#include "BTMappedDecoratorGene.h"
#include "BTChildContainerGene.h"
#include "BTChromosomeUtils.h"
#include "BTChromosome.h"
#include "ServiceInjector.h"
#include "NodeMapID.h"
#include "NodeMapper.h"

void UAddMappedDecoratorToTaskMutator::Init( UServiceInjector* injector )
{
    UMutator::Init(injector);
    mapper = serviceInjector->GetService<UNodeMapper>();
}

UBTChromosome*
UAddMappedDecoratorToTaskMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
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

    FNodeMapID newNodeID = mapper->GetRandomDecoratorID( rand );

    if (newNodeID == FNodeMapID::INVALID_ID)
    {
        // We didn't find a node to add, so just stop.
        return chromosome;
    }

    UBTMappedDecoratorGene* randomNewNode
        = NewObject<UBTMappedDecoratorGene>( parentObject, UBTMappedDecoratorGene::StaticClass() );
    randomNewNode->Init( serviceInjector, newNodeID );

    // if the parent is not empty (this isnt the root node)
    if( nullptr != parent )
	{
        UBTChildContainerGene* toAddTo = parent->GetChild( childIdx );
        toAddTo->AddDecorator( randomNewNode );
	}
    else
	{
        chromosome->AddDecorator( randomNewNode );
	}

    return chromosome;
}
