#pragma once

#include "CoreMinimal.h"
#include "Experiment/SequentialParameterModifier.h"
#include "MultiRunNoOpModifier.generated.h"
/**
 * @file MultiRunNoOpModifier.h
 */

 /**
  * @class MultiRunNoOpModifier
  * @author npcdev
  * @date 6/30/21
  * @file MultiRunNoOpModifier.h
  * @brief Does no actual parameter modification, so you can just re-run the rest of the modifiers multiple times for random initial condition restarts.
  */
UCLASS()
class EVOLVINGBEHAVIOR_API UMultiRunNoOpModifier : public USequentialParameterModifier
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere, Category="EvolvingBehavior")
	int32 NumRuns = 0;

public:

	virtual void Apply() override {};

	virtual FString GetParameterName() override { return TEXT("Run"); }

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& propertyChangedEvent) override;
#endif

};
