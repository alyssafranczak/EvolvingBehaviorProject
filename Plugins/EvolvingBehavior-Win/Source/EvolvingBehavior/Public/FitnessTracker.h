
#pragma once

#include "CoreMinimal.h"
#include "Fitness.h"
#include "Trial.h"
#include "UObject/NoExportTypes.h"
#include "Serialization/Archive.h"
#include "Service.h"

#include "FitnessTracker.generated.h"

class IFitnessUpdater;
class UFitnessCalculator;

/**
 * @file FitnessTracker.h
 */

/**
 * @class UFitnessTracker
 * @author agrundwerg
 * @author npc
 * @date 12/12/17
 * @brief Tracks fitness for all members of a population. Can be asked for a fitness score.
 * 
 * You should register every FitnessUpdater with the FitnessTracker.
 */
UCLASS(CollapseCategories)
class EVOLVINGBEHAVIOR_API UFitnessTracker : public UService
{
    GENERATED_BODY()

private:
    UPROPERTY(SaveGame)
    TMap<int32, UFitness*> fitnesses;

    UPROPERTY( Instanced )
    UFitnessCalculator* fitnessCalculator;

    float ComputeFitness( UFitness* fitness );

    float ComputeFitness(UFitness* fitness, FString fitnessKey);

public:
    void SetFitnessCalculator( UFitnessCalculator* calculator );

	bool ContainsFitnessFor(int32 id);

    void UpdateFitness( int32 id, UFitness* fitness );

    /**
     * @brief Retrieves the fitness of a specific population member (behavior tree).
     */
    UFUNCTION(BlueprintCallable, Category="EvolvingBehavior")
    float GetFitness( int32 id );
    
    UFUNCTION(BlueprintCallable, Category = "EvolvingBehavior")
    float GetFitnessValueForKey(int32 id, FString fitnessKey);

    /**
     * @brief Registers a fitness updater so this tracker will listen to updates from it.
     */
    UFUNCTION(BlueprintCallable, Category="EvolvingBehavior")
    void RegisterFitnessUpdater( TScriptInterface<IFitnessUpdater> fitnessUpdater );

    void HandleFitnessUpdated( int32 id, FString key, float value, bool isDelta );

	void Reset() { fitnesses.Empty(); }
};