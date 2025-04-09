#pragma once

#include "CoreMinimal.h"
#include "Experiment/SequentialParameterModifier.h"
#include "CrossoverProbabilityParamMod.generated.h"
/**
 * @file CrossoverProbabilityParamMod.h
 */

 /**
  * @class CrossoverProbabilityParamMod
  * @author npcdev
  * @date 6/30/21
  * @file CrossoverProbabilityParamMod.h
  * @brief Modifies probability of crossover mutators.
  */
UCLASS()
class EVOLVINGBEHAVIOR_API UCrossoverProbabilityParamMod : public USequentialParameterModifier
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, SaveGame, Category="EvolvingBehavior")
	FName PopulationTag;
	
public:

	virtual void Apply() override;

	virtual FString GetParameterName() override { return TEXT("CrossoverProb"); }

};
