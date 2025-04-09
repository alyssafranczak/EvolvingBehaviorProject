
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "ServiceInjector.h"
#include "ReplaceGeneratedDecoratorMutator.generated.h"

/**
 * @file ReplaceGeneratedDecoratorMutator.h
 */

/**
 * @class UReplaceGeneratedDecoratorMutator
 * @author jim
 * @date 11/13/19
 * @brief Replaces a random generated decorator node in the chromosome with a random one from the TemplateCollection.
 */

UCLASS()
class EVOLVINGBEHAVIOR_API UReplaceGeneratedDecoratorMutator : public UMutator
{
	GENERATED_BODY()
    
    UPROPERTY()
    UBTGeneratedTemplateLibrary* genLibrary;
	
public:
    virtual void Init( UServiceInjector* serviceInjector ) override;
	
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
