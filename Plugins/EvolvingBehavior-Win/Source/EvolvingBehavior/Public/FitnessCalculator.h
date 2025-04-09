
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Fitness.h"

#include "FitnessCalculator.generated.h"

/**
 * @file FitnessCalculator.h
 */

/**
 * @class UFitnessCalculator
 * @author npc
 * @date 12/12/17
 * @brief An abstract base class for calculating fitness scores from a tracked Fitness object.
 */
UCLASS( abstract, BlueprintType, EditInlineNew, Blueprintable, DefaultToInstanced, CollapseCategories )
class EVOLVINGBEHAVIOR_API UFitnessCalculator : public UObject
{
    GENERATED_BODY()

public:
    virtual double CalculateFinalFitness( UFitness* fitness )
        PURE_VIRTUAL( UFitnessCalculator::CalculateFinalFitness, return 0; );

    virtual double CalculateFitnessKey(UFitness* fitness, FString fitnessKey)
        PURE_VIRTUAL(UFitnessCalculator::CalculateFitnessKey, return 0; );
};
