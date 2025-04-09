#pragma once

#include "CoreMinimal.h"
#include "ExperimentManagementActor.h"

#include "SequentialParameterModifier.h"

#include "GridSearchExperimentManagementActor.generated.h"

/**
 * @file GridSearchExperimentManagementActor.h
 */

 /**
  * @class GridSearchExperimentManagementActor
  * @author npcdev
  * @date 6/30/21
  * @file GridSearchExperimentManagementActor.h
  * @brief Manages the flow of experiments to grid search over experiment parameters.
  */
UCLASS()
class EVOLVINGBEHAVIOR_API AGridSearchExperimentManagementActor : public AExperimentManagementActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Instanced, SaveGame, Category="EvolvingBehavior")
	TArray<USequentialParameterModifier*> Modifiers;

	UPROPERTY(SaveGame)
	FString OriginalExperimentID;

private:

	virtual void PostInitializeComponents() override;

	virtual void DataLoaded_Implementation() override;

	virtual void DataSaved_Implementation() override;

	virtual void HandleExperimentComplete_Implementation() override;

	void ApplyModifiers();
	
	bool IncrementModifiers();

	FString GetModifierString();
};