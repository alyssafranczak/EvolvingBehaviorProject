// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "BTEvolutionManagerComponent.h"
#include "EvolvingBehavior.h"

#include "EngineUtils.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "PopulationManager.h"
#include "EvolutionControlActor.generated.h"


UCLASS()
/**
 * @class AEvolutionControlActor
 * @file EvolutionControlActor.h
 * @brief The EvolutionControlActor owns the components that set up and control EvolvingBehavior experiments.
 */
class EVOLVINGBEHAVIOR_API AEvolutionControlActor : public AActor
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, Category = Basic)
	UBTEvolutionManagerComponent* evolutionManagerComponent;
	UPROPERTY()
	UServiceInjector* serviceInjector;

public:	
	// Sets default values for this actor's properties
	AEvolutionControlActor(const class FObjectInitializer& ObjectInitializer);

protected:

	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static TActorIterator<AEvolutionControlActor> GetEvolutionControlActors(UWorld* world) {
		TActorIterator<AEvolutionControlActor> ActorItr(world, AEvolutionControlActor::StaticClass());
		return ActorItr;
	}
	
	/**
	 * @brief Retrieves all evolution control actors in the world, and puts them in the output array (outActors).
	 */
	UFUNCTION(BlueprintCallable, Category = "EvolvingBehavior", meta=(WorldContext="worldContextObj")) 
	static void GetEvolutionControlActors(UObject* worldContextObj, TArray<AEvolutionControlActor*>& outActors);

	/**
	 * @brief Sets the name of this actors population.
	 */
	UFUNCTION(BlueprintCallable, Category = "EvolvingBehavior") 
	void SetPopulationName ( FString name ) { this->evolutionManagerComponent->SetPopulationName(name); }

	/**
	 * @brief Retrieves all evolution control actors with the specific tag provided, and puts them in the output array (outActors).
	 */
	UFUNCTION(BlueprintCallable, Category = "EvolvingBehavior", meta=(WorldContext="worldContextObj")) 
	static void GetEvolutionControlActorsByTag(UObject* worldContextObj, const FName& tag, TArray<AEvolutionControlActor*>& outActors);

	/**
	 * @brief Retrieves the evolution control actor with the specified name (or null if not found). Note that the name must be the one shown as a tooltip when hovering over the object in the scene list!
	 */
	UFUNCTION(BlueprintCallable, Category = "EvolvingBehavior", meta=(WorldContext="worldContextObj")) 
	static AEvolutionControlActor* GetEvolutionControlActor(UObject* worldContextObj, const FString& name);

	/**
	 * @brief Returns the number of the current trial (also the number of the current generation being tested).
	 */
	UFUNCTION(BlueprintCallable, Category = "EvolvingBehavior")
	int GetTrialNumber() {
		UPopulationManager* pop = serviceInjector->GetService<UPopulationManager>();
		return pop->GetCurrentTrialNumber();
	}

	/**
	 * @brief Returns the population manager that manages the population for this evolution control actor.
	 */
	UFUNCTION(BlueprintCallable, Category="EvolvingBehavior")
	UPopulationManager* GetPopulationManager();
	
	/**
	 * @brief Returns the fitness tracker that keeps fitness info for the population managed by this evolution control actor.
	 */
	UFUNCTION(BlueprintCallable, Category="EvolvingBehavior")
	UFitnessTracker* GetFitnessTracker();

	/**
	 * @brief Returns the number of seconds remaining in the current trial.
	 */
	UFUNCTION(BlueprintCallable, Category = "EvolvingBehavior")
	int SecondsRemaining();

	/**
	 * @brief Registers a given fitness updater with the fitness tracker for this evolution control actor's population. This is necessary before the fitness updater starts sending info about fitness changes.
	 */
	UFUNCTION(BlueprintCallable, Category = "EvolvingBehavior")
	void RegisterFitnessUpdater(TScriptInterface<IFitnessUpdater> fitnessUpdater);

	/**
	 * @brief Retrieve the next Behavior Tree (with its ID) to test. If none available, returns false and registers for a callback when more become available. 
	 */
	UFUNCTION(BlueprintCallable, Category = "EvolvingBehavior")
	bool TryRetrieveNextPopMember(FPopulationReg& outRegistration, const FNewTrialPopulationReadyHandler& popReadyEvent);

	UBTEvolutionManagerComponent* GetEvolutionManager()
	{
		return evolutionManagerComponent;
	}

#if WITH_EDITOR
	virtual void PostActorCreated() override;
#endif
};
