
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "ServiceInjector.h"
#include "ReplaceMappedDecoratorMutator.generated.h"

/**
 * @file ReplaceMappedDecoratorMutator.h
 */

/**
 * @class UReplaceMappedDecoratorMutator
 * @author npc
 * @date 12/12/17
 * @brief Replaces a random decorator node in the chromosome with a random one from the NodeMapper.
 */

UCLASS()
class EVOLVINGBEHAVIOR_API UReplaceMappedDecoratorMutator : public UMutator
{
	GENERATED_BODY()
    
    UPROPERTY()
    UNodeMapper* mapper;
	
public:
    virtual void Init( UServiceInjector* serviceInjector ) override;
	
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
