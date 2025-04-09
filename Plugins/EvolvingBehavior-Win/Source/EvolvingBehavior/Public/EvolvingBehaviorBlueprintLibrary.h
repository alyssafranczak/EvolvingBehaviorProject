#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "EvolvingBehaviorBlueprintLibrary.generated.h"

class AEvolutionControlActor;

/**
 * @file EvolvingBehaviorBlueprintLibrary.h
 */

/**
 * @class UEvolvingBehaviorBlueprintLibrary 
 * @author npc
 * @date 12/12/17
 * @brief Helpful utility functions for using EvolvingBehavior in blueprints.
 * 
 */
UCLASS()
class UEvolvingBehaviorBlueprintLibrary : public UBlueprintFunctionLibrary
{
GENERATED_BODY()

public:
    /**
     * @brief Returns the size of the genome (the number of nodes in the behavior tree) for the specified member from the population managed by the given evolution control actor.
     */
    UFUNCTION(BlueprintCallable, Category="EvolvingBehavior")
    static int32 GetSizeOfGenome(AEvolutionControlActor* evolutionControlActor, int32 populationMemberID);
};