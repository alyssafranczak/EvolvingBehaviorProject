
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "AddMappedServiceToCompositeMutator.generated.h"

/**
 * @file AddServiceToCompositeMutator.h
 */

/**
 * @class UAddMappedServiceToCompositeMutator
 * @author jim
 * @date 9/21/19
 * @brief Adds a random service node from the NodeMapper to a random composite node in the chromosome, if one exists.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UAddMappedServiceToCompositeMutator : public UMutator
{
    GENERATED_BODY()
    
    UPROPERTY()
    UNodeMapper* mapper;
	
public:
    void Init( UServiceInjector* injector ) override;
    
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
