
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "AddMappedDecoratorToCompositeMutator.generated.h"

/**
 * @file AddMappedDecoratorToCompositeMutator.h
 */

/**
 * @class UAddMappedDecoratorToCompositeMutator
 * @author jim
 * @date 3/24/19
 * @brief Adds a random decorator node from the NodeMapper to a random ChildContainerGene which holds a composite node in the chromosome, if one exists.
 */

UCLASS()

class EVOLVINGBEHAVIOR_API UAddMappedDecoratorToCompositeMutator : public UMutator
{
    GENERATED_BODY()
    
    UPROPERTY()
    UNodeMapper* mapper;
	
public:
    void Init( UServiceInjector* injector ) override;
    
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
