
#pragma once

#include "CoreMinimal.h"
#include "ParentSelector.h"

#include "TopNFitnessSelector.generated.h"

/**
 * @file TopNFitnessSelector.h
 */

/**
 * @class UTopNFitnessSelector
 * @author npcdev
 * @date 8/5/21
 * @brief Deterministically selects the top N most-fit individuals as parents.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UTopNFitnessSelector : public UParentSelector
{
	GENERATED_BODY()
	
	/** Select this fraction of top individuals as parents, 0-1 */
	UPROPERTY( EditAnywhere, Category = "EvolvingBehavior", meta = (ClampMin = "0.00", ClampMax = "1.00"))
	float TopNFraction = 0.2f;
	
protected:
	
public:
	void Init( float topNFraction ) { TopNFraction = topNFraction; }

    virtual TArray<FParentFitnessInfo> SelectParents( TArray<FParentFitnessInfo> parentInfo, URandomGen* randomGen );
};
