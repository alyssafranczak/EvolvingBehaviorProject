
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "ReplaceRandomDecoratorMutator.generated.h"

/**
 * @file ReplaceRandomDecoratorMutator.H
 */

/**
 * @class UReplaceRandomDecoratorMutator
 * @author jim
 * @date 3/24/19
 * @brief Replaces a random decortor node in the node mapper.
 * Chooses between mapped and generated based on variable specified in the editor.
 */

class UReplaceMappedDecoratorMutator;
class UReplaceGeneratedDecoratorMutator;

UCLASS()
class EVOLVINGBEHAVIOR_API UReplaceRandomDecoratorMutator : public UMutator
{
    GENERATED_BODY()
    
private:
    UPROPERTY( EditAnywhere, meta = (ClampMin = "0.00", ClampMax = "1.00"), Category = "EvolvingBehavior")
    double probabilityOfMapped = 0.5;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UReplaceMappedDecoratorMutator* mappedMutator;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UReplaceGeneratedDecoratorMutator* generatedMutator;
	
public:
    UReplaceRandomDecoratorMutator();

    virtual void Init( UServiceInjector* serviceInjector ) override;
    
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
