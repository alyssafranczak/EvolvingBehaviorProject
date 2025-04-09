#include "AddGeneratedDecoratorToCompositeMutator.h"

#include "EvolvingBehavior.h"

#include "BTChildContainerGene.h"
#include "BTCompositeNodeGene.h"
#include "BTDecoratorGene.h"
#include "BTGeneratedDecoratorGene.h"
#include "BTChromosome.h"
#include "BTChromosomeUtils.h"
#include "ServiceInjector.h"
#include "BTGeneratedTemplateLibrary.h"

void UAddGeneratedDecoratorToCompositeMutator::Init( UServiceInjector* injector )
{
    UMutator::Init(injector);
    genLibrary = serviceInjector->GetService<UBTGeneratedTemplateLibrary>();
}

UBTChromosome*
UAddGeneratedDecoratorToCompositeMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
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
    }
    
    // selects a random decorator to add to the tree
    UBTGeneratedDecoratorGene* newNode = genLibrary->GetRandomGeneratedDecorator( rand, parentObject );
    
    // If there are no nodes to choose from
    if( newNode == nullptr ) {
        UE_LOG( LogEvolvingBehavior, Error, TEXT( "No Generated Decorator was added (none in library)" ) );
        return chromosome;
    }
    
    // Initialize the new node, this randomozes its parameters
    newNode->RandomizeAllProperties();

    if (nullptr == newNode)
    {
        // We didn't find a node to add, so just stop.
        return chromosome;
    }
    
    // if the parent is not empty (this isnt the root node)
    if( nullptr != parent )
    {
        // Get the container of the chosen composite node
        UBTChildContainerGene* toAddTo = parent->GetChild( childIdx );
        // Add the selected decorator to this container
        toAddTo->AddDecorator( newNode );
    }
    else
    {
        // Add the selected decorator to the root container
        chromosome->AddDecorator( newNode );
    }
    
    return chromosome;
}
