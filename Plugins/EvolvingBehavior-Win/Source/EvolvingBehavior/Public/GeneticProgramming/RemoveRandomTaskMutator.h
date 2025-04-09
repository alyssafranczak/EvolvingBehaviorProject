
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "RemoveRandomTaskMutator.generated.h"

/**
 * @file RemoveRandomTaskMutator.h
 */

/**
 * @class URemoveRandomTaskMutator
 * @author jim
 * @date 3/24/19
 * @brief Removes a random task node from the chromosome if one exists.
 * Chooses between mapped and generated based on variable specified in the editor.
 */

class URemoveMappedTaskMutator;
class URemoveGeneratedTaskMutator;

UCLASS()
class EVOLVINGBEHAVIOR_API URemoveRandomTaskMutator : public UMutator
{
    GENERATED_BODY()
    
private:
    UPROPERTY( EditAnywhere, meta = (ClampMin = "0.00", ClampMax = "1.00"), Category = "EvolvingBehavior")
    double probabilityOfMapped = 0.5;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    URemoveMappedTaskMutator* mappedMutator;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    URemoveGeneratedTaskMutator* generatedMutator;
	
public:
    URemoveRandomTaskMutator();
    
    virtual void Init( UServiceInjector* serviceInjector ) override;
    
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
