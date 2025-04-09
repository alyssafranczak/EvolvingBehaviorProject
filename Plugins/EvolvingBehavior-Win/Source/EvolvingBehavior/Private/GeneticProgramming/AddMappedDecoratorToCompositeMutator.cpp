#include "AddMappedDecoratorToCompositeMutator.h"

#include "EvolvingBehavior.h"

#include "BTChildContainerGene.h"
#include "BTCompositeNodeGene.h"
#include "BTDecoratorGene.h"
#include "BTMappedDecoratorGene.h"
#include "BTChromosome.h"
#include "ServiceInjector.h"
#include "BTChromosomeUtils.h"
#include "NodeMapID.h"
#include "NodeMapper.h"

void UAddMappedDecoratorToCompositeMutator::Init(UServiceInjector* injector )
{
    UMutator::Init(injector);
    mapper = serviceInjector->GetService<UNodeMapper>();
}

UBTChromosome*
UAddMappedDecoratorToCompositeMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
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
        // Get the container of the selected node
        UBTChildContainerGene* toAddTo = parent->GetChild( childIdx );
        // Add the chosen decorator to this container
        toAddTo->AddDecorator( randomNewNode );
    }
    else
    {
        // Add the chosen decorator to the root container
        chromosome->AddDecorator( randomNewNode );
    }
    
    return chromosome;
}
