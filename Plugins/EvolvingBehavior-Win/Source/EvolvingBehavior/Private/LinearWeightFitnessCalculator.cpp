#include "LinearWeightFitnessCalculator.h"

#include "EvolvingBehavior.h"

#include "Fitness.h"
#include "Templates/Tuple.h"

double ULinearWeightFitnessCalculator::CalculateFinalFitness( UFitness* fitness )
{
    double result = 0;

    for( TTuple<FString, double>& idToWeightTuple : idToWeightMap )
	{
	    FString key = idToWeightTuple.Key;
	    double weight = idToWeightTuple.Value;

	    double value = 0;
	    if( !fitness->TryGetKey( key, value ) )
		{
		    continue;
		}

	    result += weight * value;
	}

    return result;
}

double ULinearWeightFitnessCalculator::CalculateFitnessKey(UFitness* fitness, FString fitnessKey)
{
	double result = 0;
	double* weightptr = idToWeightMap.Find(fitnessKey);

	if (weightptr == nullptr)
	{
		return 0;
	}

	double value = 0;
	if (!fitness->TryGetKey(fitnessKey, value))
	{
		return 0;
	}

	result = (*weightptr) * value;

	return result;
}
