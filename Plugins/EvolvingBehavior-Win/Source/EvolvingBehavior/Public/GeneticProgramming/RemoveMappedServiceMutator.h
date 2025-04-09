
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "RemoveMappedServiceMutator.generated.h"

/**
 * @file RemoveMappedServiceMutator.h
 */

/**
 * @class URemoveMappedServiceMutator
 * @author jim
 * @date 9/21/19
 * @brief Removes a random mapped service node from the chromosome if one exists.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API URemoveMappedServiceMutator : public UMutator
{
    GENERATED_BODY()
	
public:
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
