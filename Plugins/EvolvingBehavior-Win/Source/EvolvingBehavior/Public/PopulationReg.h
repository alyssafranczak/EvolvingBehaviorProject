#pragma once

#include "BehaviorTree/BehaviorTree.h"
#include "PopulationReg.generated.h"

/**
 * @file PopulationReg.h
 */

/**
 * @class FPopulationReg
 * @author agrundwerg
 * @date 12/12/17
 * @brief A registration of an ID and associated Behavior Tree.
 */
USTRUCT(BlueprintType)
struct FPopulationReg
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EvolvingBehavior")
	int32 id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EvolvingBehavior")
	UBehaviorTree* bt;

	FPopulationReg() = default;
	FPopulationReg(int32 n, UBehaviorTree* t)
	{
		id = n;
		bt = t;
	}

};