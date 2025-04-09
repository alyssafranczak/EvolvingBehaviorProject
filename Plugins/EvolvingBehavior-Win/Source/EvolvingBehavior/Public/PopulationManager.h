#pragma once

#include "EvolvingBehavior.h"
#include "BTEvolutionManagerComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "ChromosomeToBTConverter.h"
#include "CoreMinimal.h"
#include "Fitness.h"
#include "FitnessTracker.h"
#include "PopulationReg.h"
#include "Representation/ServiceInjector.h"
#include "Trial.h"
#include "UObject/NoExportTypes.h"
#include "Save/Saveable.h"

#include "PopulationManager.generated.h"

class ALifeCycleActor;

/**
 * @file PopulationManager.h
 */

/**
 * @class UPopulationManager
 * @author agrundwerg
 * @author npc
 * @date 12/12/17
 * @brief Tracks the population as a set of trials over time, providing information to individual members as appropriate.
 *
 * Each entity that needs a chromosome must register with the PopulationManager to get an ID.
 *
 * If the population manager is not ready, they can wait for the NewTrialPopulationReady event and then re-register.
 */

class USaveEvolvingBehavior;
class UServiceInjector;

DECLARE_DYNAMIC_DELEGATE(FNewTrialPopulationReadyHandler);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNewTrialPopulationReady);

UCLASS()
class EVOLVINGBEHAVIOR_API UPopulationManager : public UService, public ISaveable
{
    GENERATED_BODY()
    
private:
    UPROPERTY()
    TMap<int32, UTrial*> loadedPopulations;

    UPROPERTY(SaveGame)
    UFitnessTracker* tracker;

    UPROPERTY(SaveGame)
    int32 lastRegisteredID;

	UPROPERTY(SaveGame)
	int32 currentTrialNum;

	UPROPERTY()
	TArray<int32> FreedIDs;

    UPROPERTY()
    TWeakObjectPtr<UBTEvolutionManagerComponent> emc;

    /**
     * @brief Register for this event to be notified when a new population has been created.
     */
    UPROPERTY(BlueprintAssignable, Category="EvolvingBehavior")
    FNewTrialPopulationReady trialPopulationReadyEvent;
    
    void PopulateTrial( int first, int last, UTrial* trial, TArray<UBTChromosome*> trialPopulation );
    
    void BroadcastNewTrialPopulationReady() { trialPopulationReadyEvent.Broadcast(); }
    
    void UpdateInitialFitness( int32 id );
    
public:
    UPROPERTY()
    int32 TrialSize;
    UPROPERTY(EditAnywhere, Category = "Save")
    FString TreeSavePath = "/Game/EvolvingBehavior";

    void RegisterLifeCycle();

	void ResetExperiment();

    void Save(USaveEvolvingBehavior* sg);
    void Load(USaveEvolvingBehavior* sg);
	void DeleteSave();
    void SaveTrees(int32);
    void GenerateInitialPopulation();
    void LogTrialResults(int32);

	void SaveFitnessResultLog(int32 trialNum);
    
    void SetEvoManager( UBTEvolutionManagerComponent* em );
    void SetTrialSize( int32 ts );
    
    UFitnessTracker* GetTracker();
    void SetTracker( UFitnessTracker* t );
    
    void Breed(int32 trialNumber);

	int32 GetCurrentTrialNumber()
	{
		return currentTrialNum;
	};

    UTrial* GetTrial( int32 trial );

	bool AsyncLoadTrial(int32 trial);

	bool LoadTrial(int32 trial);

    UTrial* GetCurrTrial() { return GetTrial( GetCurrentTrialNumber() ); }

    /**
     * @brief Returns true if the current trial is set up and its population is created.
     */ 
    UFUNCTION(BlueprintCallable, Category="EvolvingBehavior")
    bool IsTrialReady();
	
	/**
	 * @brief Returns true if the trial is set up and its population is created.
	 */
	UFUNCTION(BlueprintCallable, Category = "EvolvingBehavior")
	bool IsTrialNumReady(int32 trialNum);
    
	FString GetSaveSlotName();

	void OnLoadedTrial(const FString & slot, const int32 userIdx, USaveGame * sg);

	FName GetTrialObjectName(int32 trialNum);

	FName GetTrialObjectName(FString saveSlotName, int32 trialNum);

	void AsyncSave();

	void Save();

	void OnSaveFinished(const FString & slot, const int32 userIdx, bool success);

    /**
     * @brief Retrieves the next unused registration (behavior tree and its ID) from the population. Returns true and puts the results in the outReg variable, or returns false if no registration was available.
     */
    UFUNCTION(BlueprintCallable, Category="EvolvingBehavior")
	bool Register( FPopulationReg& outReg );

	UFUNCTION(BlueprintCallable, Category="EvolvingBehavior")
	void ReleaseRegistration(int32 id);

	UFUNCTION(BlueprintCallable, Category = "EvolvingBehavior")
	bool IsRegistered(int32 id);
    
    FNewTrialPopulationReady& OnTrialPopulationReady() { return trialPopulationReadyEvent; }

    UFUNCTION(BlueprintCallable, Category="EvolvingBehavior")
    int32 GetPopulationRemaining();

    UFUNCTION(BlueprintCallable, Category="EvolvingBehavior")
    int32 GetNumSpawnedThisTrial();
	
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable Actor")
    void DataLoaded();
    virtual void DataLoaded_Implementation() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Saveable Actor")
    void DataSaved();
    virtual void DataSaved_Implementation() override;
};
