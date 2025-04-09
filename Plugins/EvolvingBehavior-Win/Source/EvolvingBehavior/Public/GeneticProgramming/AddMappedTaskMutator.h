#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "AddMappedTaskMutator.generated.h"

/**
 * @file AddMappedTaskMutator.h
 */

/**
 * @class UAddMappedTaskMutator
 * @author jim
 * @date 3/24/19
 * @brief Adds a random task node from the NodeMapper to a randome composite node in the chromosome,
 * or to the root if none exist.
 */

UCLASS()
class EVOLVINGBEHAVIOR_API UAddMappedTaskMutator : public UMutator
{
    GENERATED_BODY()
    
    UPROPERTY()
    UNodeMapper* mapper;
	
public:
    void Init( UServiceInjector* injector ) override;
    
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
