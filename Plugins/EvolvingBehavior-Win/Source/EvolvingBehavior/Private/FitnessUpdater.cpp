#include "FitnessUpdater.h"

#include "EvolvingBehavior.h"

// Add default functionality here for any IIFitnessUpdater functions that are not pure virtual.
void IFitnessUpdater::BroadcastFitnessUpdated( int32 objectID, const FString& key, float value, bool isDelta )
{
	FitnessUpdatedEvent.Broadcast( objectID, key, value, isDelta );
}