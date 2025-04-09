
#pragma once

#include "CoreMinimal.h"
#include "ParentSelector.h"

#include "TournamentSelector.generated.h"

/**
 * @file TournamentSelector.h
 */

/**
 * @class UTournamentSelector
 * @author isrivastava
 * @date 12/12/17
 * @brief An implementation of tournament selection, with a variable to set the number of members who participate in the tournament.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UTournamentSelector : public UParentSelector
{
	GENERATED_BODY()
	
	UPROPERTY( EditAnywhere, meta=(ClampMin=2), Category = "EvolvingBehavior")
	int32 NumCandidates = 4;
	
protected:
	int32 GetMax( TArray<FParentFitnessInfo> parents );
	
public:
	void Init( int32 num ) { NumCandidates = num; }

    virtual TArray<FParentFitnessInfo> SelectParents( TArray<FParentFitnessInfo> parentInfo, URandomGen* randomGen );
};
