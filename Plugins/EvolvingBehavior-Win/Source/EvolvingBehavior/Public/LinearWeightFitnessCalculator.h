
#pragma once

#include "CoreMinimal.h"
#include "FitnessCalculator.h"
#include "LinearWeightFitnessCalculator.generated.h"

/**
 * @file LinearWeightFitnessCalculator.h
 */

/**
 * @class ULinearWeightFitnessCalculator
 * @author npc
 * @date 12/12/17
 * @brief Calculates a data-driven linear combination of fitness values.
 */
UCLASS(CollapseCategories)
class EVOLVINGBEHAVIOR_API ULinearWeightFitnessCalculator : public UFitnessCalculator
{
    GENERATED_BODY()
	
protected:
	UPROPERTY( EditAnywhere, Category = "EvolvingBehavior")
	TMap<FString, double> idToWeightMap;
	
public:
    virtual double CalculateFinalFitness( UFitness* fitness ) override;
	virtual double CalculateFitnessKey(UFitness* fitness, FString fitnessKey) override;
};
