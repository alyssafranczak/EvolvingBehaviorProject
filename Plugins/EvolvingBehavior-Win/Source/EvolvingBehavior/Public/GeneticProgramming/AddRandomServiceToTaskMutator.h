
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "AddRandomServiceToTaskMutator.generated.h"

/**
 * @file AddRandomServiceToTaskMutator.h
 */

/**
 * @class UAddRandomServiceToTaskMutator
 * @author jim
 * @date 3/24/19
 * @brief Adds a random service to a random task node in the chromosome.
 * Chooses between mapped and generated based on variable specified in the editor
 */

class UAddMappedServiceToTaskMutator;
class UAddGeneratedServiceToTaskMutator;

UCLASS()
class EVOLVINGBEHAVIOR_API UAddRandomServiceToTaskMutator : public UMutator
{
    GENERATED_BODY()
    
private:
    UPROPERTY( EditAnywhere, meta = (ClampMin = "0.00", ClampMax = "1.00"), Category = "EvolvingBehavior")
    double probabilityOfMapped = 0.5;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UAddMappedServiceToTaskMutator* mappedMutator;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UAddGeneratedServiceToTaskMutator* generatedMutator;
	
public:
    UAddRandomServiceToTaskMutator();
    
    virtual void Init( UServiceInjector* serviceInjector ) override;
    
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
