#include "TopNFitnessSelector.h"

#include "EvolvingBehavior.h"

#include "Containers/Array.h"
#include "ParentSelector.h"
#include "RandomGen.h"

TArray<FParentFitnessInfo> UTopNFitnessSelector::SelectParents( TArray<FParentFitnessInfo> parentInfo,
    URandomGen* randomGen )
{
	const TArray<FParentFitnessInfo>& parentInfoRef = parentInfo;
	// Sort parents in reverse fitness order.
	TArray<FParentFitnessInfo> sortedParents(parentInfoRef);
	sortedParents.Sort([](const FParentFitnessInfo& parent1, const FParentFitnessInfo& parent2)
	{
		return parent1.fitness < parent2.fitness;
	});

	// Take the top N (but at least one, and no more than all parents).
	int32 topN = FMath::Min(FMath::Max(int32(FMath::Floor(sortedParents.Num() * TopNFraction)), 1), sortedParents.Num());

	TArray<FParentFitnessInfo> selectedParents;
	for (int i = 0; i < topN; ++i)
	{
		selectedParents.Add(sortedParents.Pop());
	}

    return selectedParents;
}
