
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "AddRandomDecoratorToTaskMutator.generated.h"

/**
 * @file AddRandomDecoratorToTaskMutator.h
 */

/**
 * @class UAddRandomDecoratorToTaskMutator
 * @author jim
 * @date 3/24/19
 * @brief Adds a random decorator to a random ChildContainerGene which holds a task node in the chromosome,
 * if one exists. Chooses between mapped and generated based on variable specified in the editor
 */

class UAddMappedDecoratorToTaskMutator;
class UAddGeneratedDecoratorToTaskMutator;

UCLASS()
class EVOLVINGBEHAVIOR_API UAddRandomDecoratorToTaskMutator : public UMutator
{
    GENERATED_BODY()
    
private:
    UPROPERTY( EditAnywhere, meta = (ClampMin = "0.00", ClampMax = "1.00"), Category = "EvolvingBehavior")
    double probabilityOfMapped = 0.5;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UAddMappedDecoratorToTaskMutator* mappedMutator;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UAddGeneratedDecoratorToTaskMutator* generatedMutator;
	
public:
    UAddRandomDecoratorToTaskMutator();
    
    virtual void Init( UServiceInjector* serviceInjector ) override;
    
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
