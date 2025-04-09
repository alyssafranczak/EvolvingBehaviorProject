#include "AddMappedServiceToCompositeMutator.h"

#include "EvolvingBehavior.h"

#include "BTChildContainerGene.h"
#include "BTMappedServiceGene.h"
#include "BTChromosome.h"
#include "BTChromosomeUtils.h"
#include "ServiceInjector.h"
#include "NodeMapper.h"

void UAddMappedServiceToCompositeMutator::Init( UServiceInjector* injector )
{
    UMutator::Init(injector);
    mapper = serviceInjector->GetService<UNodeMapper>();
}

UBTChromosome*
UAddMappedServiceToCompositeMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    UBTCompositeNodeGene* parent = nullptr;
    UBTNodeGene* compositeNode = nullptr;
    uint16 childIdx;

    URandomGen* rand = serviceInjector->GetService<URandomGen>();
    
    // Sets compositeNode to a random composite node from the current behavior tree. Parent is the composite node who owns it, and childIdx is the index of the container holding the found node within the parent
    BTChromosomeUtils::FindRandomNodeOfType( chromosome, rand, UBTCompositeNodeGene::StaticClass(), parent, childIdx, compositeNode );
     
    UObject* parentObject = nullptr;
    
    if( nullptr != parent )
    {
        parentObject = parent->GetChild( childIdx );
    }
    else
    {
        parentObject = chromosome;
        compositeNode = chromosome->GetRoot();
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

    Cast<UBTCompositeNodeGene>( compositeNode )->AddService( randomNewNode );
    
    return chromosome;
}
