
#pragma once

#include "CoreMinimal.h"
#include "FitnessUpdater.h"
#include "Components/ActorComponent.h"
#include "SingleIDFitnessUpdater.generated.h"

/**
 * @file SingleIDFitnessUpdater.h
 */

/**
 * @class USingleIDFitnessUpdater
 * @author npc
 * @date 12/12/17
 * @brief An implementation of IFitnessUpdater for objects that send fitness events about a single ID to the FitnessTracker.
 * 
 * You should register all FitnessUpdaters with the tracker.
 */
UCLASS(Blueprintable, ClassGroup= ( Custom ), meta = ( BlueprintSpawnableComponent ))
class EVOLVINGBEHAVIOR_API USingleIDFitnessUpdater : public UActorComponent, public IFitnessUpdater
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	int32 objectID = -1;

public:

	USingleIDFitnessUpdater() {}

	UFUNCTION(BlueprintCallable, Category="EvolvingBehavior")
	void SetID( int32 newID )
	{
		objectID = newID;
	}
	
	UFUNCTION(BlueprintCallable, Category="EvolvingBehavior")
	void BroadcastFitnessUpdate( const FString& key, float value, bool isDelta = false);
};