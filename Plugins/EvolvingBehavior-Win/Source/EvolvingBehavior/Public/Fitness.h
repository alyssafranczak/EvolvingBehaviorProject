
#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Fitness.generated.h"

/**
 * @file Fitness.h
 */

/**
 * @class UFitness
 * @author agrundwerg
 * @author npc
 * @date 12/12/17
 * @brief A fitness tracker, containing keys mapped to fitness values per key.
 * 
 * This will be compiled by the FitnessCalculator into a final score.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UFitness : public UObject
{
    GENERATED_BODY()

private:
    UPROPERTY(SaveGame)
    TMap<FString, double> fitnessKeys;

public:
    void UpdateKey( FString key, double newValue ) { fitnessKeys.Add( key, newValue ); }

    void UpdateKeyDelta( FString key, double newValue );

    bool TryGetKey( FString key, double& value );
};
