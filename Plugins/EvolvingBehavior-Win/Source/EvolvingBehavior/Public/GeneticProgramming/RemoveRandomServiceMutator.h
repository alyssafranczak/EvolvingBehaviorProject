
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "RemoveRandomServiceMutator.generated.h"

/**
 * @file RemoveRandomServiceMutator.h
 */

/**
 * @class URemoveRandomServiceMutator
 * @author jim
 * @date 3/24/19
 * @brief Removes a random service node from the chromosome if one exists.
 * Chooses between mapped and generated based on variable specified in the editor.
 */

class URemoveMappedServiceMutator;
class URemoveGeneratedServiceMutator;

UCLASS()
class EVOLVINGBEHAVIOR_API URemoveRandomServiceMutator : public UMutator
{
    GENERATED_BODY()
    
private:
    UPROPERTY( EditAnywhere, meta = (ClampMin = "0.00", ClampMax = "1.00"), Category = "EvolvingBehavior")
    double probabilityOfMapped = 0.5;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    URemoveMappedServiceMutator* mappedMutator;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    URemoveGeneratedServiceMutator* generatedMutator;
	
public:
    URemoveRandomServiceMutator();
    
    virtual void Init( UServiceInjector* serviceInjector ) override;
    
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
