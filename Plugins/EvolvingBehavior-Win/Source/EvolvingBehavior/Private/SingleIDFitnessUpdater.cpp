#include "SingleIDFitnessUpdater.h"

#include "EvolvingBehavior.h"

void USingleIDFitnessUpdater::BroadcastFitnessUpdate( const FString& key, float value, bool isDelta)
{
    if ( objectID < 0 )
    {
        UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Object ID not set when trying to update fitness: %s"), *key );
        return;
    }

    BroadcastFitnessUpdated(objectID, key, value, isDelta);
}