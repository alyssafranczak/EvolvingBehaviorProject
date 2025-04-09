
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "ReplaceRandomTaskMutator.generated.h"

/**
 * @file ReplaceRandomDecoratorMutator.h
 */

/**
 * @class UReplaceRandomTaskMutator
 * @author jim
 * @date 3/24/19
 * @brief Replaces a random task node in the node mapper.
 * Chooses between mapped and generated based on variable specified in the editor.
 */

class UReplaceMappedTaskMutator;
class UReplaceGeneratedTaskMutator;

UCLASS()
class EVOLVINGBEHAVIOR_API UReplaceRandomTaskMutator : public UMutator
{
    GENERATED_BODY()
    
private:
    UPROPERTY( EditAnywhere, meta = (ClampMin = "0.00", ClampMax = "1.00"), Category = "EvolvingBehavior")
    double probabilityOfMapped = 0.5;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UReplaceMappedTaskMutator* mappedMutator;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UReplaceGeneratedTaskMutator* generatedMutator;
	
public:
    UReplaceRandomTaskMutator();

    virtual void Init( UServiceInjector* serviceInjector ) override;

    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
