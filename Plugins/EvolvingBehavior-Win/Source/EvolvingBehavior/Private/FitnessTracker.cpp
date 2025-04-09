#include "FitnessTracker.h"

#include "EvolvingBehavior.h"
#include "LinearWeightFitnessCalculator.h"
#include "FitnessCalculator.h"
#include "FitnessUpdater.h"

void UFitnessTracker::SetFitnessCalculator( UFitnessCalculator* calculator )
{ 
	if (nullptr == calculator) 
	{		
		fitnessCalculator = NewObject<ULinearWeightFitnessCalculator>(this, TEXT("Default Calculator"));
		UE_LOG(LogEvolvingBehaviorError, Warning, TEXT("Warning: Using a default FitnessCalculator. You should set your own fitness calculator in the life cycle component. Your fitness value is set to 0, which leads to changing behavior trees without a specific goal."));
	}
	else 
	{
		calculator->Rename(NULL, this);
		fitnessCalculator = calculator;
	}
}

bool UFitnessTracker::ContainsFitnessFor(int32 id)
{
	return fitnesses.Contains(id);
}

float UFitnessTracker::ComputeFitness( UFitness* fitness )
{
    ensure( nullptr != fitnessCalculator );

    float score = fitnessCalculator->CalculateFinalFitness( fitness );
    return score;
}

float UFitnessTracker::ComputeFitness(UFitness* fitness, FString fitnessKey)
{
	ensure(nullptr != fitnessCalculator);

	float score = fitnessCalculator->CalculateFitnessKey(fitness, fitnessKey);
	return score;
}

float UFitnessTracker::GetFitness( int32 id )
{
    if( !fitnesses.Contains( id ) )
	{
	    return 0.0;
	}
    return ComputeFitness( fitnesses[id] );
}

float UFitnessTracker::GetFitnessValueForKey(int32 id, FString fitnessKey)
{
	if (!fitnesses.Contains(id))
	{
		return 0.0;
	}
	return ComputeFitness(fitnesses[id], fitnessKey);
}

void UFitnessTracker::RegisterFitnessUpdater( TScriptInterface<IFitnessUpdater> fitnessUpdater )
{
    fitnessUpdater->OnFitnessUpdated().AddUObject( this, &UFitnessTracker::HandleFitnessUpdated );
}

void UFitnessTracker::UpdateFitness( int32 id, UFitness* fitness ) { fitnesses.Add( id, fitness ); }

void UFitnessTracker::HandleFitnessUpdated( int32 id, FString key, float value, bool isDelta )
{
    UFitness** fitness = fitnesses.Find( id );
    if( nullptr != fitness )
	{
	    if( isDelta )
		{
		    ( *fitness )->UpdateKeyDelta( key, value );
		}
	    else
		{
		    ( *fitness )->UpdateKey( key, value );
		}
	}
}
