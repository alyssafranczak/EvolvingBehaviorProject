#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FitnessUpdater.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFitnessUpdater : public UInterface
{
	GENERATED_BODY()
};

/**
 * @file FitnessUpdater.h
 */

/**
 * @class IFitnessUpdater
 * @author npc
 * @date 12/12/17
 * @brief An interface for objects that send fitness events to the FitnessTracker.
 * 
 * You should register all FitnessUpdaters with the tracker.
 */
class EVOLVINGBEHAVIOR_API IFitnessUpdater
{
	GENERATED_BODY()

public:
	DECLARE_EVENT_FourParams( IFitnessUpdater, FFitnessUpdatedEvent, int32, FString, float, bool );
	
	FFitnessUpdatedEvent& OnFitnessUpdated() { return FitnessUpdatedEvent; }
	
private:
	FFitnessUpdatedEvent FitnessUpdatedEvent;

protected:

	void BroadcastFitnessUpdated( int32 objectID, const FString& key, float value, bool isDelta = false );
	
};
