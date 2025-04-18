#include "AddGeneratedServiceToCompositeMutator.h"

#include "EvolvingBehavior.h"

#include "BTChildContainerGene.h"
#include "BTGeneratedServiceGene.h"
#include "BTChromosome.h"
#include "BTChromosomeUtils.h"
#include "ServiceInjector.h"
#include "BTGeneratedTemplateLibrary.h"

void UAddGeneratedServiceToCompositeMutator::Init(UServiceInjector* injector )
{
    UMutator::Init(injector);
    genLibrary = serviceInjector->GetService<UBTGeneratedTemplateLibrary>();
}

UBTChromosome*
UAddGeneratedServiceToCompositeMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
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
    UBTGeneratedServiceGene* newNode = genLibrary->GetRandomGeneratedService( rand, parentObject );
    
    if( newNode == nullptr ) {
        UE_LOG( LogEvolvingBehavior, Error, TEXT( "No Generated Service was added (none in library)" ) );
        return chromosome;
    }
    
    // mutates node so it starts with random values in its range
    newNode->RandomizeAllProperties();
    
    if (nullptr == newNode)
    {
        // We didn't find a service to add, so just stop.
        return chromosome;
    }

    Cast<UBTCompositeNodeGene>( compositeNode )->AddService( newNode );
    
    return chromosome;
}
