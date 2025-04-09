
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "RemoveMappedTaskMutator.generated.h"

/**
 * @file RemoveMappedTaskMutator.h
 */

/**
 * @class URemoveMappedTaskMutator
 * @author jim
 * @date 3/24/19
 * @brief Removes a random mapped task node from the chromosome if one exists.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API URemoveMappedTaskMutator : public UMutator
{
    GENERATED_BODY()
	
public:
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
