#include "AddGeneratedTaskMutator.h"

#include "EvolvingBehavior.h"

#include "BTChildContainerGene.h"
#include "BTChromosomeUtils.h"
#include "BTCompositeNodeGene.h"
#include "BTGeneratedTaskGene.h"
#include "ServiceInjector.h"
#include "BTGeneratedTemplateLibrary.h"

void UAddGeneratedTaskMutator::Init( UServiceInjector* injector )
{
    UMutator::Init(injector);
    genLibrary = serviceInjector->GetService<UBTGeneratedTemplateLibrary>();
}

UBTChromosome*
UAddGeneratedTaskMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
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
    
    UBTChildContainerGene* childContainer = NewObject<UBTChildContainerGene>(compositeNode, UBTChildContainerGene::StaticClass());

    check(nullptr != genLibrary);
    UBTGeneratedTaskGene* newNode = genLibrary->GetRandomGeneratedTask( rand, childContainer );
    
    if( newNode == nullptr ) {
        UE_LOG( LogEvolvingBehavior, Error, TEXT( "No Generated Task was added (none in library)" ) );
        return chromosome;
    }
    
    // mutates node so it starts with random values in its range
    newNode->RandomizeAllProperties();

    if (nullptr == newNode)
    {
        // We didn't find a task to add, so just stop.
        return chromosome;
    }
    
    childContainer->SetChild( newNode );

    Cast<UBTCompositeNodeGene>(compositeNode)->AddChild( childContainer );
    
    return chromosome;
}
