#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "PerturbPropertyInServiceMutator.generated.h"

/**
 * @file PerturbPropertyInServiceMutator.h
 */

/**
 * @class UPerturbPropertyInServiceMutator
 * @author npcdev
 * @date 10/18/21
 * @brief Randomly modifies a property value in a generated service node.
 * 
 * If numeric, by adding a random number sampled from a gaussian distribution.
 * 
 * Otherwise, it will pick a new random value from the available options.
 */

UCLASS()
class EVOLVINGBEHAVIOR_API UPerturbPropertyInServiceMutator : public UMutator
{
    GENERATED_BODY()

    /** How strong the perturbation should be - the higher the value the more likely it will be to cause large changes (more technically, this is the standard deviation of the normal/gaussian distribution as a fraction of the total property range) */
    UPROPERTY(EditAnywhere, meta = (ClampMin = "0.00", ClampMax = "1.00"), Category="EvolvingBehavior")
    float ScaleOfPerturbation = 0.1f;
    
public:
    void Init( UServiceInjector* injector ) override;
    
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
