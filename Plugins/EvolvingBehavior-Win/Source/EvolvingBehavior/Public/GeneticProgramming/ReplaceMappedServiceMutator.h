
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "ServiceInjector.h"
#include "ReplaceMappedServiceMutator.generated.h"

/**
 * @file ReplaceMappedServiceMutator.h
 */

/**
 * @class UReplaceMappedServiceMutator
 * @author jim
 * @date 9/21/19
 * @brief Replaces a random service node in the chromosome with a random one from the NodeMapper.
 */

UCLASS()
class EVOLVINGBEHAVIOR_API UReplaceMappedServiceMutator : public UMutator
{
	GENERATED_BODY()
    
    UPROPERTY()
    UNodeMapper* mapper;
	
public:
    virtual void Init( UServiceInjector* serviceInjector ) override;
	
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
