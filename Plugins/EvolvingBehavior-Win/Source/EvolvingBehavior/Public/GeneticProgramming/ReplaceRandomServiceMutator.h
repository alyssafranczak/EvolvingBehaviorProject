
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "ReplaceRandomServiceMutator.generated.h"

/**
 * @file ReplaceRandomServiceMutator.H
 */

/**
 * @class UReplaceRandomServiceMutator
 * @author jim
 * @date 3/24/19
 * @brief Replaces a random service node in the node mapper.
 * Chooses between mapped and generated based on variable specified in the editor.
 */

class UReplaceMappedServiceMutator;
class UReplaceGeneratedServiceMutator;

UCLASS()
class EVOLVINGBEHAVIOR_API UReplaceRandomServiceMutator : public UMutator
{
    GENERATED_BODY()
    
private:
    UPROPERTY( EditAnywhere, meta = (ClampMin = "0.00", ClampMax = "1.00"), Category = "EvolvingBehavior")
    double probabilityOfMapped = 0.5;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UReplaceMappedServiceMutator* mappedMutator;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UReplaceGeneratedServiceMutator* generatedMutator;
	
public:
    UReplaceRandomServiceMutator();

    virtual void Init( UServiceInjector* serviceInjector ) override;
    
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
