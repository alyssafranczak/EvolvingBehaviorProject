#include "ReplaceMappedServiceMutator.h"

#include "EvolvingBehavior.h"

#include "NodeMapID.h"
#include "NodeMapper.h"
#include "BTServiceGene.h"
#include "BTMappedServiceGene.h"
#include "BTChromosome.h"
#include "BTChromosomeUtils.h"
#include "BTChildContainerGene.h"

void UReplaceMappedServiceMutator::Init( UServiceInjector* injector )
{
    UMutator::Init(injector);
    mapper = serviceInjector->GetService<UNodeMapper>();
}

UBTChromosome*
UReplaceMappedServiceMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    UBTCompositeNodeGene* parent = nullptr;
    UBTNodeGene* oldNode = nullptr;
    uint16 childIdx;

    URandomGen* rand = serviceInjector->GetService<URandomGen>();
    
    // Sets oldNode to a mapped service node from the current behavior tree. Parent is the composite node who owns it, and childIdx is the index of the container holding the found node within the parent
    BTChromosomeUtils::FindRandomNodeOfType(
        chromosome, rand, UBTMappedServiceGene::StaticClass(), parent, childIdx, oldNode );

    if( nullptr == oldNode )
	{
		// We didn't find any Services - probably there are none in the Chromosome. This is ok.
		return chromosome;
	}

    UBTServiceGene* oldService = Cast<UBTServiceGene>( oldNode );
    if( nullptr == oldService )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Error: found a non-Service to replace? Node: %s" ),
	        *oldNode->ToString() );
	}
    
    FNodeMapID newNodeID = mapper->GetRandomServiceID( rand );
    
    if (newNodeID == FNodeMapID::INVALID_ID)
    {
        // We didn't find a node to add, so just stop.
        return chromosome;
    }

    
    //FIX ME (maybe abstract for service nodes / task+composite)
    if( nullptr == parent )
	{
        UBTCompositeNodeGene* serviceOwner = chromosome->GetRoot();
        UBTMappedServiceGene* randomNewNode
        = NewObject<UBTMappedServiceGene>( serviceOwner, UBTMappedServiceGene::StaticClass() );
        randomNewNode->Init( serviceInjector, newNodeID );
        
        int32 oldServiceIndex = serviceOwner->RemoveService( oldService );
        if( oldServiceIndex < 0 )
        {
            UE_LOG( LogEvolvingBehaviorError, Error,
                   TEXT( "Error: could not remove Service - not found in parent: %s " ), *oldService->ToString() );
            return chromosome;
        }
        serviceOwner->InsertService( randomNewNode, oldServiceIndex );
	}
    else {
        UBTChildContainerGene* geneChild = parent->GetChild(childIdx);
        if(geneChild->IsALeaf()) {
            UBTTaskNodeGene* serviceOwner = geneChild->GetTaskChild();
            UBTMappedServiceGene* randomNewNode
            = NewObject<UBTMappedServiceGene>( serviceOwner, UBTMappedServiceGene::StaticClass() );
            randomNewNode->Init( serviceInjector, newNodeID );
            
            int32 oldServiceIndex = serviceOwner->RemoveService( oldService );
            if( oldServiceIndex < 0 )
            {
                UE_LOG( LogEvolvingBehaviorError, Error,
                       TEXT( "Error: could not remove Service - not found in parent: %s " ), *oldService->ToString() );
                return chromosome;
            }
            serviceOwner->InsertService( randomNewNode, oldServiceIndex );
        }
        else {
            UBTCompositeNodeGene* serviceOwner = geneChild->GetCompositeChild();
            UBTMappedServiceGene* randomNewNode
            = NewObject<UBTMappedServiceGene>( serviceOwner, UBTMappedServiceGene::StaticClass() );
            randomNewNode->Init( serviceInjector, newNodeID );
            
            int32 oldServiceIndex = serviceOwner->RemoveService( oldService );
            if( oldServiceIndex < 0 )
            {
                UE_LOG( LogEvolvingBehaviorError, Error,
                       TEXT( "Error: could not remove Service - not found in parent: %s " ), *oldService->ToString() );
                return chromosome;
            }
            serviceOwner->InsertService( randomNewNode, oldServiceIndex );
        }
    }
    
    return chromosome;
}
