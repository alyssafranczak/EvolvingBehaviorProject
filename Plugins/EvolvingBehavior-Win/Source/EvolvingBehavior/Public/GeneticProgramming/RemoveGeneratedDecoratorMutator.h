
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "RemoveGeneratedDecoratorMutator.generated.h"

/**
 * @file RemoveGeneratedDecoratorMutator.h
 */

/**
 * @class URemoveGeneratedDecoratorMutator
 * @author jim
 * @date 11/13/19
 * @brief Removes a random generated decorator node from the chromosome if one exists.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API URemoveGeneratedDecoratorMutator : public UMutator
{
    GENERATED_BODY()
	
public:
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
