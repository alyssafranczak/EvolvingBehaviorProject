#include "ReplaceGeneratedServiceMutator.h"

#include "EvolvingBehavior.h"

#include "BTServiceGene.h"
#include "BTGeneratedServiceGene.h"
#include "BTChromosome.h"
#include "BTChromosomeUtils.h"
#include "BTChildContainerGene.h"
#include "BTGeneratedTemplateLibrary.h"

void UReplaceGeneratedServiceMutator::Init( UServiceInjector* injector )
{
    UMutator::Init(injector);
    genLibrary = serviceInjector->GetService<UBTGeneratedTemplateLibrary>();
}

UBTChromosome*
UReplaceGeneratedServiceMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    UBTCompositeNodeGene* parent = nullptr;
    UBTNodeGene* oldNode = nullptr;
    uint16 childIdx;
    
    URandomGen* rand = serviceInjector->GetService<URandomGen>();
    
    // Sets oldNode to a generated service node from the current behavior tree. Parent is the composite node who owns it, and childIdx is the index of the container holding the found node within the parent
    BTChromosomeUtils::FindRandomNodeOfType(
                                            chromosome, rand, UBTGeneratedServiceGene::StaticClass(), parent, childIdx, oldNode );
    
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
    
    //FIX ME (maybe abstract for service nodes / task+composite)
    if( nullptr == parent )
    {
        UBTCompositeNodeGene* serviceOwner = chromosome->GetRoot();
        
        UBTGeneratedServiceGene* randomNewNode = genLibrary->GetRandomGeneratedService( rand, serviceOwner );
        randomNewNode->RandomizeAllProperties();
        
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
            
            UBTGeneratedServiceGene* randomNewNode = genLibrary->GetRandomGeneratedService( rand, serviceOwner );
            randomNewNode->RandomizeAllProperties();
            
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
            
            UBTGeneratedServiceGene* randomNewNode = genLibrary->GetRandomGeneratedService( rand, serviceOwner );
            randomNewNode->RandomizeAllProperties();
            
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
