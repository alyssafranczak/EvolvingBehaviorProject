#include "AddMappedServiceToTaskMutator.h"

#include "EvolvingBehavior.h"

#include "BTChildContainerGene.h"
#include "BTMappedServiceGene.h"
#include "BTChromosome.h"
#include "BTChromosomeUtils.h"
#include "ServiceInjector.h"
#include "NodeMapper.h"

void UAddMappedServiceToTaskMutator::Init( UServiceInjector* injector )
{
    UMutator::Init(injector);
    mapper = serviceInjector->GetService<UNodeMapper>();
}

UBTChromosome*
UAddMappedServiceToTaskMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
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
        //parentObject = chromosome;
        //taskNode = chromosome->GetRoot();
        return chromosome;
    }
    
    // selects a random service to add to the tree
    FNodeMapID newNodeID = mapper->GetRandomServiceID( rand );
    if (newNodeID == FNodeMapID::INVALID_ID)
    {
        // We didn't find a service to add, so just stop.
        return chromosome;
    }

    UBTMappedServiceGene* randomNewNode = NewObject<UBTMappedServiceGene>( parentObject, UBTMappedServiceGene::StaticClass() );
    randomNewNode->Init( serviceInjector, newNodeID );

    Cast<UBTTaskNodeGene>( taskNode )->AddService( randomNewNode );
    
    return chromosome;
}
