
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "AddRandomTaskMutator.generated.h"

/**
 * @file AddRandomTaskMutator.h
 */

/**
 * @class UAddRandomTaskMutator
 * @author jim
 * @date 3/24/19
 * @brief Adds a random task node to the chromosome.
 * Chooses between mapped and generated based on variable specified in the editor
 */

class UAddMappedTaskMutator;
class UAddGeneratedTaskMutator;

UCLASS()
class EVOLVINGBEHAVIOR_API UAddRandomTaskMutator : public UMutator
{
    GENERATED_BODY()
    
private:
    UPROPERTY( EditAnywhere, meta = (ClampMin = "0.00", ClampMax = "1.00"), Category = "EvolvingBehavior")
    double probabilityOfMapped = 0.5;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UAddMappedTaskMutator* mappedMutator;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UAddGeneratedTaskMutator* generatedMutator;
	
public:
    UAddRandomTaskMutator();
    
    virtual void Init( UServiceInjector* serviceInjector ) override;

    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
