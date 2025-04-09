
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "AddGeneratedDecoratorToTaskMutator.generated.h"

/**
 * @file AddGeneratedDecoratorToTaskMutator.h
 */

/**
 * @class UAddGeneratedDecoratorToTaskMutator
 * @author jim
 * @date 11/13/19
 * @brief Adds a random generated decorator from the TemplateCollection to a random ChildContainerGene 
 * which contains a task node in the chromosome, if one exists.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UAddGeneratedDecoratorToTaskMutator : public UMutator
{
    GENERATED_BODY()
    
    UPROPERTY()
    UBTGeneratedTemplateLibrary* genLibrary;
	
public:
    void Init( UServiceInjector* injector ) override;
    
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
