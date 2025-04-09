
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "RemoveGeneratedServiceMutator.generated.h"

/**
 * @file RemoveServiceMutator.h
 */

/**
 * @class URemoveGeneratedServiceMutator
 * @author jim
 * @date 11/13/19
 * @brief Removes a random generated service node from the chromosome if one exists.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API URemoveGeneratedServiceMutator : public UMutator
{
    GENERATED_BODY()
	
public:
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
