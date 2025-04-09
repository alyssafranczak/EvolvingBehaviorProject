
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "ServiceInjector.h"
#include "ReplaceMappedTaskMutator.generated.h"

/**
 * @file ReplaceMappedTaskMutator.h
 */

/**
 * @class UReplaceMappedTaskMutator
 * @author npc
 * @date 12/12/17
 * @brief Replaces a random task node in the chromosome with a random one from the node mapper.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UReplaceMappedTaskMutator : public UMutator
{
    GENERATED_BODY()
    
    UPROPERTY()
    UNodeMapper* mapper;
	
public:
    virtual void Init( UServiceInjector* serviceInjector ) override;
   
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
