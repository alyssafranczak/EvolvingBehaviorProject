
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "RemoveGeneratedTaskMutator.generated.h"

/**
 * @file RemoveTaskMutator.h
 */

/**
 * @class URemoveGeneratedTaskMutator
 * @author jim
 * @date 11/13/19
 * @brief Removes a random generated task node from the chromosome if one exists.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API URemoveGeneratedTaskMutator : public UMutator
{
    GENERATED_BODY()
	
public:
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
