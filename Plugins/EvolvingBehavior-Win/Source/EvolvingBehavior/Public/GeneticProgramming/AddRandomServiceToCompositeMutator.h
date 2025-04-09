
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "AddRandomServiceToCompositeMutator.generated.h"

/**
 * @file AddRandomServiceToCompositeMutator.h
 */

/**
 * @class UAddRandomServiceToCompositeMutator
 * @author jim
 * @date 3/24/19
 * @brief Adds a random service to a random composite node in the chromosome.
 * Chooses between mapped and generated based on variable specified in the editor
 */

class UAddMappedServiceToCompositeMutator;
class UAddGeneratedServiceToCompositeMutator;

UCLASS()
class EVOLVINGBEHAVIOR_API UAddRandomServiceToCompositeMutator : public UMutator
{
    GENERATED_BODY()
    
private:
    UPROPERTY( EditAnywhere, meta = (ClampMin = "0.00", ClampMax = "1.00"), Category = "EvolvingBehavior")
    double probabilityOfMapped = 0.5;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UAddMappedServiceToCompositeMutator* mappedMutator;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UAddGeneratedServiceToCompositeMutator* generatedMutator;
	
public:
    UAddRandomServiceToCompositeMutator();
    
    virtual void Init( UServiceInjector* serviceInjector ) override;
    
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
