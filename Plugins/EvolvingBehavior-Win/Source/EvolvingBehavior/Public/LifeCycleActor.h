
#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Representation/ServiceInjector.h"
#include "PopulationManager.h"
#include "Save/SaveEvolvingBehavior.h"
#include "GameFramework/Actor.h"
#include "EngineUtils.h"

#include "LifeCycleActor.generated.h"

class UFitnessCalculator;
class URandomGen;

/**
 * @file LifeCycleComponent.h
 */

/**
 * @class ALifeCycleActor
 * @author agrundwerg
 * @date 12/12/17
 * @file LifeCycleActor.h
 * @brief Is the LifeCycle settings and passes them to the appropriate managers.
 */
UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class EVOLVINGBEHAVIOR_API ALifeCycleActor : public AActor
{
    GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "EvolvingBehavior")
	int TrialLength = 60;
	UPROPERTY(EditAnywhere, Category = "EvolvingBehavior")
	int32 TotalTrials = 10;

	UPROPERTY(EditAnywhere, Category = "EvolvingBehavior")
	double GameSpeedModifier = 1.0;

	DECLARE_EVENT(ALifeCycleActor, FGenerateInitialPopulationEvent);
	FGenerateInitialPopulationEvent& OnGenerateInitialPopulation() { return GenerateInitialPopulationEvent; }

	DECLARE_EVENT_OneParam(ALifeCycleActor, FOnNextTrialEvent, int32);
	FOnNextTrialEvent& OnNextTrial() { return OnNextTrialEvent; }

	DECLARE_EVENT_OneParam(ALifeCycleActor, FLogTrialResultsEvent, int32);
	FLogTrialResultsEvent& OnLogTrialResults() { return LogTrialResultsEvent; }

	DECLARE_EVENT_OneParam(ALifeCycleActor, FSaveTreesEvent, int32);
	FSaveTreesEvent& OnSaveTrees() { return SaveTreesEvent; }

	DECLARE_EVENT_OneParam(ALifeCycleActor, FSaveEvent, USaveEvolvingBehavior*);
	FSaveEvent& OnSave() { return SaveEvent; }

	DECLARE_EVENT_OneParam(ALifeCycleActor, FLoadEvent, USaveEvolvingBehavior*);
	FLoadEvent& OnLoad() { return LoadEvent; }

	DECLARE_EVENT(ALifeCycleActor, FExperimentCompleteEvent);
	FExperimentCompleteEvent& OnExperimentComplete() { return ExperimentCompleteEvent; }

	DECLARE_EVENT(ALifeCycleActor, FResetExperimentEvent);
	FResetExperimentEvent& OnResetExperiment() { return ResetExperimentEvent; }

	DECLARE_EVENT(ALifeCycleActor, FDeleteSaveEvent);
	FDeleteSaveEvent& OnDeleteSave() { return DeleteSaveEvent; }

private:
	UPROPERTY()
	URandomGen* rand;

	int32 trialNumber = -1;

	FString experimentID = TEXT("00000000");

	bool waitingForTrial = false;

	bool experimentEnding = false;

	bool noSaveAvailable = false;

	UPROPERTY(EditAnywhere, Category = "Save And Load")
	FString SlotName = TEXT("DefaultSavedTrial");

	UPROPERTY(EditAnywhere, Category = "Save And Load")
	int32 UserIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Map")
	FName ExperimentEndingMap = TEXT("/Game/Maps/MainMenu_Entry.MainMenu_Entry");

	float trialStartTime;

	FGenerateInitialPopulationEvent GenerateInitialPopulationEvent;
	FLogTrialResultsEvent LogTrialResultsEvent;
	FSaveTreesEvent SaveTreesEvent;
	FSaveEvent SaveEvent;
	FLoadEvent LoadEvent;
	FOnNextTrialEvent OnNextTrialEvent;
	FExperimentCompleteEvent ExperimentCompleteEvent;
	FResetExperimentEvent ResetExperimentEvent;
	FDeleteSaveEvent DeleteSaveEvent;

	void GenerateExperimentID();

public:
    // Sets default values for this component's properties
    ALifeCycleActor();

	UFUNCTION(BlueprintCallable, Category = "EvolvingBehavior")
	static ALifeCycleActor* GetLifeCycleActor(UWorld* world) {
		TActorIterator<ALifeCycleActor> ActorItr(world, ALifeCycleActor::StaticClass(), EActorIteratorFlags::SkipPendingKill);
		for (; ActorItr; ++ActorItr)
		{
			// Return the first valid actor.
			return *ActorItr;
		}
		
		// We failed to find a valid LifeCycle actor
		return nullptr;
	} 

	void RestartExperiment();

	int32 IncrementTrial() { return ++trialNumber; }

	void CheckIfTrialTimeExpired();

	float GetSecondsRemaining();

	void NextTrial(); // end this round collect data and stuff

	void EndExperiment();

	void LogTrialResults();

	void SaveTrees();

	void AsyncSave();

	FString GetSaveSlotName() { return SlotName; }

	UFUNCTION()
	void OnSaveFinished(const FString& slot, const int32 userIdx, bool bsuccess);

	void AsyncLoad();

	UFUNCTION()
	void Load(const FString& slot, const int32 userIdx, USaveGame* sg);

	void StartTrial();

	void DeleteSave(const FString& slot, const int32 userIdx);

	UFUNCTION(CallInEditor, Category = "Save And Load")
	void DeleteCurrentSave() { DeleteSave(SlotName, UserIndex); }

protected:
	// Called before game starts
	virtual void PreInitializeComponents() override;
	
	// Called when the game starts
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void
    Tick( float DeltaTime) override;

	FString GetExperimentID() { return FString::Printf(TEXT("%s-%i-%s"),*SlotName,UserIndex,*this->experimentID); }

	FString GetExperimentSubID() { return this->experimentID; }

	virtual void SetExperimentID(FString newID) { experimentID = newID; }

};
