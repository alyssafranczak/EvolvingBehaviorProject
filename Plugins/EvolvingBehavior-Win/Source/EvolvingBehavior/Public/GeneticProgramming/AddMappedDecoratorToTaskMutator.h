
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "AddMappedDecoratorToTaskMutator.generated.h"

/**
 * @file AddDecoratorToTaskMutator.h
 */

/**
 * @class UAddMappedDecoratorToTaskMutator
 * @author jim
 * @date 3/24/19
 * @brief Adds a random decorator node from the NodeMapper to a random ChildContainerGene which holds a task 
 * node in the chromosome, if one exists.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UAddMappedDecoratorToTaskMutator : public UMutator
{
    GENERATED_BODY()
    
    UPROPERTY()
    UNodeMapper* mapper;
	
public:
    void Init( UServiceInjector* injector ) override;
    
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
