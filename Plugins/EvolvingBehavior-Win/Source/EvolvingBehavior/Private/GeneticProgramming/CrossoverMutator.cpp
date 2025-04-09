#include "CrossoverMutator.h"

#include "EvolvingBehavior.h"

#include "BTChromosomeUtils.h"
#include "RandomGen.h"
#include "BTChildContainerGene.h"
#include "ServiceInjector.h"

// returns a behavior tree given a list of possible crossover trees and a primary tree to mutate
UBTChromosome* UCrossoverMutator::Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
{
    UBTCompositeNodeGene* ourParentNode = nullptr;
    UBTNodeGene* ourOldNode = nullptr;
    uint16 ourChildIdx;
    
    URandomGen* rand = serviceInjector->GetService<URandomGen>();

    // Choose task or composite crossover:
    bool ourCrossoverIsTask = ShouldCrossoverTaskNode( chromosome );

    // Chooses a random task node from the tree
    if( ourCrossoverIsTask )
	{
	    BTChromosomeUtils::FindRandomNodeOfType(
	        chromosome, rand, UBTTaskNodeGene::StaticClass(), ourParentNode, ourChildIdx, ourOldNode );
	}
    // Otherwise chooses a random composite
    else
	{
	    BTChromosomeUtils::FindRandomNodeOfType(
	        chromosome, rand, UBTCompositeNodeGene::StaticClass(), ourParentNode, ourChildIdx, ourOldNode );
	}

    // if chosen node has no parent (i.e. is the root) abandon mutation and return original
    UObject* parentObject = ourParentNode;
    if( nullptr == parentObject )
	{
	    // We would be replacing the root... don't bother.
	    return chromosome;
	}

    // get the compositegenechild contaning our chosen node
    UBTChildContainerGene* ourChildContainer = ourParentNode->GetChild( ourChildIdx );

    // chooses the parent tree to cross with the primary tree
    UBTChromosome* parentChromosome = SelectParentChromosome( parents, primaryParent );

    // Choose composite or task crossover from the parent:
    bool theirCrossoverIsTask = ShouldCrossoverTaskNode( parentChromosome );

    // chooses a random task node from the selected parent and preforms the crossover
    if( theirCrossoverIsTask )
	{
	    CrossoverWithTheirTask( parentChromosome, ourChildContainer );
	}
    // otherwise chooses a random composite node and preforms the crossover
    else
	{
	    CrossoverWithTheirComposite( parentChromosome, ourChildContainer, chromosome );
	}
    // returns the newlsy crossed cromosome
    return chromosome;
}

void UCrossoverMutator::CrossoverWithTheirTask( UBTChromosome* parentChromosome,
    UBTChildContainerGene* ourChildContainer )
{
    UBTCompositeNodeGene* theirParentNode = nullptr;
    UBTNodeGene* theirNode = nullptr;
    uint16 theirChildIdx;
    
    URandomGen* rand = serviceInjector->GetService<URandomGen>();

    BTChromosomeUtils::FindRandomNodeOfType(
        parentChromosome, rand, UBTTaskNodeGene::StaticClass(), theirParentNode, theirChildIdx, theirNode );
    if( nullptr == theirNode )
	{
	    // There was nothing to crossover from the other parent. The parent is probably nearly empty, so don't bother.
	    return;
	}

    UBTTaskNodeGene* theirNodeTask = Cast<UBTTaskNodeGene>( theirNode );

    UBTTaskNodeGene* theirNodeCopy = theirNodeTask->DuplicateTask( ourChildContainer );
	
    // replaces the child of the selected node (task or composite) from the primary parent with the
    // selected task node from the selected secondary parent
    ourChildContainer->ReplaceChild( theirNodeCopy );
    return;
}

void UCrossoverMutator::CrossoverWithTheirComposite( UBTChromosome* parentChromosome,
    UBTChildContainerGene* ourChildContainer,
    UBTChromosome* ourChromosome )
{
    UBTCompositeNodeGene* theirParentNode = nullptr;
    UBTNodeGene* theirNode = nullptr;
    uint16 theirChildIdx;

    URandomGen* rand = serviceInjector->GetService<URandomGen>();

    BTChromosomeUtils::FindRandomNodeOfType(
        parentChromosome, rand, UBTCompositeNodeGene::StaticClass(), theirParentNode, theirChildIdx, theirNode );
    if( nullptr == theirNode )
	{
	    // There was nothing to crossover from the other parent. The parent is probably nearly empty, so don't bother.
	    return;
	}
     
    UBTCompositeNodeGene* theirNodeComposite = Cast<UBTCompositeNodeGene>( theirNode );

    UBTCompositeNodeGene* theirNodeCopy = theirNodeComposite->DuplicateComposite( ourChildContainer );
	
    // replaces the child of the selected node (task or composite) from the primary parent with the
    // selected composite node from the selected secondary parent
    ourChildContainer->ReplaceChild( theirNodeCopy );

	return;
}

// determines whether crossover should be preformed with a task node
bool UCrossoverMutator::ShouldCrossoverTaskNode( UBTChromosome* chromosome )
{
    URandomGen* rand = serviceInjector->GetService<URandomGen>();

    // numTask is the number of task nodes in the tree
    int numTask = BTChromosomeUtils::CountNumNodesOfType( chromosome, UBTTaskNodeGene::StaticClass() );

    // numComposite is the number of composite nodes in the tree
    int numComposite = BTChromosomeUtils::CountNumNodesOfType( chromosome, UBTCompositeNodeGene::StaticClass() );

    //returns the fraction of task nodes (if there are 10 tasks and 15 composites has a 10/25 chance
    // to return true
    return rand->WithProbability( (double)numTask / ( (double)( numTask + numComposite ) ) );
}

// selects a tree to cross with this one given a list of possible parents and this one
UBTChromosome* UCrossoverMutator::SelectParentChromosome( TArray<UBTChromosome*> parents, int primaryParent )
{
    URandomGen* rand = serviceInjector->GetService<URandomGen>();

    // parentIdx is the idx of a random parent from the list of possibilities
    int32 parentIdx;
    
    do {
        parentIdx = rand->UniformIntInRange( 0, parents.Num() - 1 );
    }
    while( parents.Num() > 1 && parentIdx == primaryParent );
    
    // returns the chromosome of the selected parent
    return parents[parentIdx];
}
