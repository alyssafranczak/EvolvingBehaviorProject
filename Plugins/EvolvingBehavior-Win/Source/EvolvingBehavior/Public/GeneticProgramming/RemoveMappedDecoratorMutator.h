
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "RemoveMappedDecoratorMutator.generated.h"

/**
 * @file RemoveDecoratorMutator.h
 */

/**
 * @class URemoveMappedDecoratorMutator
 * @author jim
 * @date 3/24/19
 * @brief Removes a random mapped decorator node from the chromosome if one exists.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API URemoveMappedDecoratorMutator : public UMutator
{
    GENERATED_BODY()
	
public:
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
