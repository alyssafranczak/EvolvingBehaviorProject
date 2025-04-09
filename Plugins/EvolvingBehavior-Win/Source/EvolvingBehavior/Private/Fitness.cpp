#include "Fitness.h"

#include "EvolvingBehavior.h"


void UFitness::UpdateKeyDelta( FString key, double newValue )
{
    double& value = fitnessKeys.FindOrAdd( key );
    value += newValue;
}
bool UFitness::TryGetKey( FString key, double& value )
{
    if( !fitnessKeys.Contains( key ) )
	{
	    return false;
	}

    value = fitnessKeys[key];
    return true;
}
