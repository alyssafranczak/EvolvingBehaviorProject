#include "DepthBiasedCrossoverMutator.h"

#include <vector>
#include <algorithm>
#include <iostream>
#include <math.h>

#include "EvolvingBehavior.h"
#include "RandomGen.h"

#include "BTChildContainerGene.h"
#include "BTChromosomeUtils.h"
#include "ServiceInjector.h"

// returns a behavior tree given a list of possible crossover trees and a primary tree to mutate
UBTChromosome* UDepthBiasedCrossoverMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    UBTCompositeNodeGene* ourParentNode = nullptr;
    UBTNodeGene* ourOldNode = nullptr;
    uint16 ourChildIdx;

    // Choose layer to swap from
    int ourSelectedLayer = SelectLayer( chromosome );
    
    BTChromosomeUtils::PickNodeAtLayer( chromosome, ourSelectedLayer, ourParentNode, ourChildIdx, ourOldNode );

    // if chosen node has no parent (i.e. is the root) abandon mutation and return original
    if( nullptr == ourParentNode )
	{
	    // We would be replacing the root... don't bother.
	    return chromosome;
	}
    
    // chooses the parent tree to cross with the primary tree
    UBTChromosome* parentChromosome = SelectParentChromosome( parents, primaryParent );
    
    UBTCompositeNodeGene* theirParentNode = nullptr;
    UBTNodeGene* theirOldNode = nullptr;
    uint16 theirChildIdx;
    
    int theirSelectedLayer = SelectLayer( parentChromosome );
    
    BTChromosomeUtils::PickNodeAtLayer( parentChromosome, theirSelectedLayer, theirParentNode, theirChildIdx, theirOldNode );
    
    if( nullptr == theirParentNode )
    {
        // We did not find a valid node
        return chromosome;
    }
    
    UBTChildContainerGene* ourChildContainer = ourParentNode->GetChild( ourChildIdx );
    
    UBTChildContainerGene* theirChildContainer = theirParentNode->GetChild( theirChildIdx );

    // chooses a random task node from the selected parent and preforms the crossover
    CrossoverWithTheirNode( ourChildContainer, theirChildContainer );
    
    // returns the newly crossed cromosome
    return chromosome;
}

void UDepthBiasedCrossoverMutator::CrossoverWithTheirNode( UBTChildContainerGene* ourChildContainer,
    UBTChildContainerGene* theirChildContainer )
{
    if (theirChildContainer->IsALeaf())
    {
        UBTTaskNodeGene* theirChildCopy = theirChildContainer->GetTaskChild()->DuplicateTask( ourChildContainer );
        ourChildContainer->ReplaceChild( theirChildCopy );
    }
    else
    {
        UBTCompositeNodeGene* theirChildCopy = theirChildContainer->GetCompositeChild()->DuplicateComposite( ourChildContainer );
        ourChildContainer->ReplaceChild( theirChildCopy );
    }
}

int UDepthBiasedCrossoverMutator::SelectLayer( UBTChromosome* chromosome )
{
    URandomGen* rand = serviceInjector->GetService<URandomGen>();

    // We don't ever want to select the root, because crossover with the root is just a no-op.
    // Depth is also 1 higher than the maximum layer, so subtract 1.
    int maxLayer = BTChromosomeUtils::DepthOfTree( chromosome ) - 1;

    // Randomly choose a linear value between 0 to 1, which we will then
    // adjust by an exponent based on our bias strength.
    double x = rand->UniformRealInRange( 0, 1 );

    // Bias with exponent based on our bias strength (e.g. 2 would be quadratic),
    // and round down to nearest integer layer.
    // Add one to the depth, so it can't be 0 (so we don't select the root node).
    int selectedLayer = FMath::FloorToInt(maxLayer * pow(1-x, biasStrength) + 1);
    
    if (selectedLayer > maxLayer)
    {
        selectedLayer = maxLayer;
    }
    
    return selectedLayer;
}

// selects a tree to cross with this one given a list of possible parents and this one
UBTChromosome* UDepthBiasedCrossoverMutator::SelectParentChromosome( TArray<UBTChromosome*> parents, int primaryParent )
{
    URandomGen* rand = serviceInjector->GetService<URandomGen>();

    // parentIdx is the idx of a random parent from the list of possibilities
    int32 parentIdx;
    
    do {
        parentIdx = rand->UniformIntInRange( 0, parents.Num() - 1 );
    }
    while( parentIdx == primaryParent );
    
    // returns the chromosome of the selected parent
    return parents[parentIdx];
}
