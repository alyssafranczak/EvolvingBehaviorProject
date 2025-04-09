
#pragma once

#include "CoreMinimal.h"
#include "Representation/BTChromosome.h"
#include <functional>
#include <queue>

class UBTCompositeNodeGene;
class URandomGen;
class UBTNodeGene;
class UBTChromosome;

/**
 * @file BTChromosomeUtils.h
 */

/**
 * @class BTChromosomeUtils
 * @author npc
 * @date 12/12/17
 * @brief Helpful utility functions for our chromosomes.
 * 
 */
class EVOLVINGBEHAVIOR_API BTChromosomeUtils
{

public:
    static bool FindRandomNodeOfType( UBTChromosome* chromosome,
        URandomGen* random,
        UClass* nodeClass,
        UBTCompositeNodeGene*& outParent,
        uint16& outChildIdx,
        UBTNodeGene*& resultNode );

    template<typename NodeT>
	static bool FindRandomNode(UBTChromosome* chromosome,
		URandomGen* randomGen,
		UBTCompositeNodeGene*& outParent,
		uint16& outChildIdx,
		NodeT*& resultNode)
	{
		ensure(chromosome != nullptr);
		ensure(randomGen != nullptr);

		bool foundChild = false;

		if (nullptr == chromosome->GetRoot())
		{
			UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Error: null root!"));
			return false;
		}

		int numValidNodes = BTChromosomeUtils::CountNumNodes<NodeT>(chromosome);

		bool rootIsDesiredType = (Cast<NodeT>(chromosome->GetRoot()) != nullptr);

		int32_t randomNodeIdx = randomGen->UniformIntInRange(1, numValidNodes);

		if (randomNodeIdx == 1 && rootIsDesiredType)
		{
			// Return false to indicate that we randomly selected the root node, which has no parent nor child index.
			return false;
		}

		int numNodes = randomNodeIdx;
		DepthFirstSearch(chromosome, [&](UBTNodeGene* node, UBTCompositeNodeGene* parent, uint16 childIdx)
		{
			if (nullptr == node)
			{
				UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Error: null node in parent: %i"), childIdx);
				return true;
			}

			NodeT* nodeAsType = Cast<NodeT>(node);
			if (nodeAsType)
			{
				numNodes--;
				if (numNodes <= 0)
				{
					outParent = parent;
					outChildIdx = childIdx;
					resultNode = nodeAsType;
					return false;
				}
			}
			return true;
		});

		return foundChild;
	}

    static int CountNumNodesOfType( UBTChromosome* chromosome, UClass* nodeClass );
    
    template<typename NodeT>
    static int CountNumNodes( UBTChromosome* chromosome )
	{
		int numNodes = 0;
		DepthFirstSearch(chromosome, [&](UBTNodeGene* node, UBTCompositeNodeGene* compositeParent, uint16 childIdx)
		{
			if (Cast<NodeT>(node))
			{
				numNodes++;
			}
			return true;
		});

		return numNodes;
	}
	
	/**
	 * @brief Performs a depth-first search on the given chromsome, calling a given function on each node.
	 * @param chromosome The chromosome to search.
	 * @param nodeFunc The function to call on each node in the chromosome.
	 */
    static void DepthFirstSearch( UBTChromosome* chromosome,
        std::function<bool( UBTNodeGene*, UBTCompositeNodeGene*, uint16 )> nodeFunc );

    static bool DepthFirstSearch( UBTCompositeNodeGene* composite,
        UBTCompositeNodeGene* parent,
        uint16 compositeIdx,
        std::function<bool( UBTNodeGene*, UBTCompositeNodeGene*, uint16 )> nodeFunc,
        TArray<UBTNodeGene*> seenNodes );

    static bool ContainsACycle( UBTChromosome* chromosome );

	static int DepthOfTree( UBTChromosome* chromosome );

	static bool CheckForPointerCollisions( UBTCompositeNodeGene* composite, UBTCompositeNodeGene* other );
	static bool CheckForPointerCollisions( UBTChromosome* chromosome, UBTChromosome* other );

    static bool PickNodeAtLayer( UBTChromosome* chromosome, int layer, UBTCompositeNodeGene*& outParent, uint16& outChildIdx, UBTNodeGene*& resultNode);

private:

    static int DepthOfTreeHelp( UBTChromosome* chromosome, UBTCompositeNodeGene* composite, int maxDepth );

    
    static void BreadthFirstSearch( UBTChromosome* chromosome, std::function<bool( UBTNodeGene*, UBTCompositeNodeGene*, uint16, uint16 )> nodeFunc );
    static bool BreadthFirstSearchInternal( std::queue< std::pair< UBTCompositeNodeGene*, uint16> > q, std::function<bool( UBTNodeGene*, UBTCompositeNodeGene*, uint16, uint16 )> nodeFunc );
};
