#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"
#include "Save/Saveable.h"

#include "ExperimentManagementActor.generated.h"

class USaveEvolvingBehavior;

/**
 * @file ExperimentManagementActor.h
 */

 /**
  * @class ExperimentManagementActor
  * @author npcdev
  * @date 6/30/21
  * @file ExperimentManagementActor.h
  * @brief Manages the flow of experiments (sets up the experiment, acts on experiment completion). For instance, can be used to set up a sequence of experiments to tune parameters.
  */
UCLASS(abstract, ClassGroup = (Custom), BlueprintType, Blueprintable, DefaultToInstanced)
class EVOLVINGBEHAVIOR_API AExperimentManagementActor : public AActor, public ISaveable
{
	GENERATED_BODY()

private:


public:
	AExperimentManagementActor();

	virtual void PreInitializeComponents() override;

	virtual void PostInitializeComponents() override;

	virtual void Save(USaveEvolvingBehavior* sg);

	virtual void Load(USaveEvolvingBehavior* sg);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable Actor")
		void DataLoaded();
	virtual void DataLoaded_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable Actor")
		void DataSaved();
	virtual void DataSaved_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, Category = "ExperimentManagement")
		void HandleExperimentComplete();
	virtual void HandleExperimentComplete_Implementation();
};