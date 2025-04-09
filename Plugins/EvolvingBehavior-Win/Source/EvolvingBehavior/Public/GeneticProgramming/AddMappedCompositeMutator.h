
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "AddMappedCompositeMutator.generated.h"

/**
 * @file AddMappedCompositeMutator.h
 */

/**
 * @class UAddMappedCompositeMutator
 * @author jim
 * @date 3/24/19
 * @brief Adds a random composite node from the NodeMapper to the chromosome. This will be inserted under a
 * random composite node gene in the chromosome already, or under the root node if none exist.
 */

UCLASS()
class EVOLVINGBEHAVIOR_API UAddMappedCompositeMutator : public UMutator
{
    GENERATED_BODY()
    
    UPROPERTY()
    UNodeMapper* mapper;
	
public:
    void Init( UServiceInjector* injector ) override;
    
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
