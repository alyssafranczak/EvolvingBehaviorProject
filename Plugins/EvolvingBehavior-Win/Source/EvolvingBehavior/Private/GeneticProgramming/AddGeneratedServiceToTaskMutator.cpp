#include "AddGeneratedServiceToTaskMutator.h"

#include "EvolvingBehavior.h"

#include "BTChildContainerGene.h"
#include "BTGeneratedServiceGene.h"
#include "BTChromosome.h"
#include "BTChromosomeUtils.h"
#include "NodeMapper.h"
#include "ServiceInjector.h"
#include "BTGeneratedTemplateLibrary.h"

void UAddGeneratedServiceToTaskMutator::Init(UServiceInjector* injector )
{
    UMutator::Init(injector);
    genLibrary = serviceInjector->GetService<UBTGeneratedTemplateLibrary>();
}

UBTChromosome*
UAddGeneratedServiceToTaskMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
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

    Cast<UBTTaskNodeGene>( taskNode )->AddService( newNode );
    
    return chromosome;
}
