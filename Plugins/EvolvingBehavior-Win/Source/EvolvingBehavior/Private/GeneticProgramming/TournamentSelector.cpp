#include "TournamentSelector.h"

#include "EvolvingBehavior.h"

#include "ParentSelector.h"
#include "RandomGen.h"
#include <limits>

TArray<FParentFitnessInfo> UTournamentSelector::SelectParents( TArray<FParentFitnessInfo> parentInfo,
    URandomGen* randomGen )
{
    TArray<FParentFitnessInfo> ProbableCandidates;
    TArray<FParentFitnessInfo> FinalisedParents;
    for( int i = 0; i < NumCandidates; i++ )
	{
	    ProbableCandidates.Add( parentInfo[( randomGen->UniformIntInRange( 0, ( parentInfo.Num() - 1 ) ) )] );
	}

    int32 bestIndex = GetMax( ProbableCandidates );
    FinalisedParents.Add( ProbableCandidates[bestIndex] );

    ProbableCandidates.RemoveAt( bestIndex );

    FinalisedParents.Add( ProbableCandidates[GetMax( ProbableCandidates )] );

    return FinalisedParents;
}

int32 UTournamentSelector::GetMax( TArray<FParentFitnessInfo> parents )
{
    double max = std::numeric_limits<double>::min();
    int32 bestIndex = 0;

    for( int i = 0; i < parents.Num(); ++i )
	{
	    if( parents[i].fitness > max )
		{
		    max = parents[i].fitness;
		    bestIndex = i;
		}
	}

    return bestIndex;
}
