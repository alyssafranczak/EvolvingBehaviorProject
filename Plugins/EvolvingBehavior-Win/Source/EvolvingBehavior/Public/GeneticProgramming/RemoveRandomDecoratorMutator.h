
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "RemoveRandomDecoratorMutator.generated.h"

/**
 * @file RemoveRandomDecoratorMutator.h
 */

/**
 * @class URemoveRandomDecoratorMutator
 * @author jim
 * @date 3/24/19
 * @brief Removes a random decorator node from the chromosome if one exists.
 * Chooses between mapped and generated based on variable specified in the editor.
 */

class URemoveMappedDecoratorMutator;
class URemoveGeneratedDecoratorMutator;

UCLASS()
class EVOLVINGBEHAVIOR_API URemoveRandomDecoratorMutator : public UMutator
{
    GENERATED_BODY()
    
private:
    UPROPERTY( EditAnywhere, meta = (ClampMin = "0.00", ClampMax = "1.00"), Category = "EvolvingBehavior")
    double probabilityOfMapped = 0.5;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    URemoveMappedDecoratorMutator* mappedMutator;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    URemoveGeneratedDecoratorMutator* generatedMutator;
	
public:
    URemoveRandomDecoratorMutator();
    
    virtual void Init( UServiceInjector* serviceInjector ) override;
    
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
