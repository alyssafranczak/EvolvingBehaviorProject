#pragma once

#include "CoreMinimal.h"
#include "Save/Saveable.h"

#include "SequentialParameterModifier.generated.h"

/**
 * @file SequentialParameterModifier.h
 */

 /**
  * @class SequentialParameterModifier
  * @author npcdev
  * @date 6/30/21
  * @file SequentialParameterModifier.h
  * @brief Manages modification of a particular parameter over a sequence.
  */
UCLASS(abstract, Blueprintable, EditInlineNew, BlueprintType, DefaultToInstanced, CollapseCategories)
class EVOLVINGBEHAVIOR_API USequentialParameterModifier : public UObject
{
	GENERATED_BODY()

private:

	UPROPERTY(SaveGame)
	int32 CurrNumApplied = 0;

protected:

	UPROPERTY(EditAnywhere, SaveGame, Category="EvolvingBehavior")
	TArray<float> ApplicationValues;

protected:
	float GetCurrentValue();

public:

	virtual void Apply() {}

	void Restart();

	virtual FString GetParameterName() { return FString(); }

	virtual FString GetShortString();

	bool Increment();
};