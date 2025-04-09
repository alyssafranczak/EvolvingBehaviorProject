#include "ReplaceGeneratedTaskMutator.h"

#include "EvolvingBehavior.h"

#include "BTCompositeNodeGene.h"
#include "BTNodeGene.h"
#include "BTChromosomeUtils.h"
#include "BTGeneratedTaskGene.h"
#include "BTChildContainerGene.h"
#include "BTGeneratedTemplateLibrary.h"

void UReplaceGeneratedTaskMutator::Init( UServiceInjector* injector )
{
    UMutator::Init(injector);
    genLibrary = serviceInjector->GetService<UBTGeneratedTemplateLibrary>();
}

UBTChromosome*
UReplaceGeneratedTaskMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    UBTCompositeNodeGene* parent = nullptr;
	UBTNodeGene* oldNode = nullptr;
    uint16 childIdx;

    URandomGen* rand = serviceInjector->GetService<URandomGen>();
    
    // Sets oldNode to a generated task node from the current behavior tree. Parent is the composite node who owns it, and childIdx is the index of the container holding the found node within the parent
    BTChromosomeUtils::FindRandomNodeOfType( chromosome, rand, UBTGeneratedTaskGene::StaticClass(), parent, childIdx, oldNode );

    if( nullptr == parent )
	{
	    UE_LOG( LogEvolvingBehavior, Display, TEXT( "No generated task to replace found in chromosome: %s" ),
	        *chromosome->ToString() );
	    return chromosome;
	}
    
    UBTChildContainerGene* childContainer = parent->GetChild( childIdx );

    UBTGeneratedTaskGene* newNode = genLibrary->GetRandomGeneratedTask( rand, childContainer );
    
    if( newNode == nullptr ) {
        return chromosome;
    }
    
    // mutates node so it starts with random values in its range
    newNode->RandomizeAllProperties();

    if (nullptr == newNode)
    {
        // We didn't find a node to add, so just stop.
        return chromosome;
    }

    childContainer->ReplaceChild( newNode );
	
	return chromosome;
}
