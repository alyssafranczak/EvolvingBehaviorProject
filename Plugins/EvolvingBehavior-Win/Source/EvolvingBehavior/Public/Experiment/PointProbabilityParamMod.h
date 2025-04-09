#pragma once

#include "CoreMinimal.h"
#include "Experiment/SequentialParameterModifier.h"
#include "PointProbabilityParamMod.generated.h"
/**
 * @file PointProbabilityParamMod.h
 */

 /**
  * @class PointProbabilityParamMod
  * @author npcdev
  * @date 6/30/21
  * @file PointProbabilityParamMod.h
  * @brief Modifies probability of point mutators.
  */
UCLASS()
class EVOLVINGBEHAVIOR_API UPointProbabilityParamMod : public USequentialParameterModifier
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, SaveGame, Category="EvolvingBehavior")
	FName PopulationTag;
	
public:

	virtual void Apply() override;

	virtual FString GetParameterName() override { return TEXT("PointProb"); }

};
