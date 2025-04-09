
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "AddGeneratedServiceToTaskMutator.generated.h"

/**
 * @file AddGeneratedServiceToTaskMutator.h
 */

/**
 * @class UAddGeneratedServiceToTaskMutator
 * @author jim
 * @date 11/13/19
 * @brief Adds a random generated service node from the TemplateCollection to a random task node in the 
 * chromosome, if one exists.
 */

UCLASS()
class EVOLVINGBEHAVIOR_API UAddGeneratedServiceToTaskMutator : public UMutator
{
    GENERATED_BODY()
    
    UPROPERTY()
    UBTGeneratedTemplateLibrary* genLibrary;
	
public:
    void Init( UServiceInjector* injector ) override;
    
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
