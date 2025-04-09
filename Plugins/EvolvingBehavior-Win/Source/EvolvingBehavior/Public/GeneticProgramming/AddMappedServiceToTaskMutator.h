
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "AddMappedServiceToTaskMutator.generated.h"

/**
 * @file AddMappedServiceToTaskMutator.h
 */

/**
 * @class UAddMappedServiceToTaskMutator
 * @author jim
 * @date 9/21/19
 * @brief Adds a random service node from the NodeMapper to a random task node in the chromosome, if one exists.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UAddMappedServiceToTaskMutator : public UMutator
{
    GENERATED_BODY()
    
    UPROPERTY()
    UNodeMapper* mapper;
	
public:
    void Init( UServiceInjector* injector ) override;
 
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
