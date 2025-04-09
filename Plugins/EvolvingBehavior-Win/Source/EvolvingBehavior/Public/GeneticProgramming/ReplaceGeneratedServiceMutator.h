
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "ServiceInjector.h"
#include "ReplaceGeneratedServiceMutator.generated.h"

/**
 * @file ReplaceServiceMutator.h
 */

/**
 * @class UReplaceGeneratedServiceMutator
 * @author npc
 * @date 11/13/19
 * @brief Replaces a random generated service node in the chromosome with a random one from the TemplateCollection.
 */

UCLASS()
class EVOLVINGBEHAVIOR_API UReplaceGeneratedServiceMutator : public UMutator
{
	GENERATED_BODY()
    
    UPROPERTY()
    UBTGeneratedTemplateLibrary* genLibrary;
	
public:
    virtual void Init( UServiceInjector* serviceInjector ) override;
	
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
