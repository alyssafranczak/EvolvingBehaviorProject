
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "AddRandomDecoratorToCompositeMutator.generated.h"

/**
 * @file AddRandomDecoratorToCompositeMutator.h
 */

/**
 * @class UAddRandomDecoratorToCompositeMutator
 * @author jim
 * @date 3/24/19
 * @brief Adds a random decorator to a random ChildContainerGene which holds a composite node in the chromosome,
 * if one exists. Chooses between mapped and generated based on variable specified in the editor
 */

class UAddMappedDecoratorToCompositeMutator;
class UAddGeneratedDecoratorToCompositeMutator;

UCLASS()
class EVOLVINGBEHAVIOR_API UAddRandomDecoratorToCompositeMutator : public UMutator
{
    GENERATED_BODY()
    
private:
    UPROPERTY( EditAnywhere, meta = (ClampMin = "0.00", ClampMax = "1.00"), Category = "EvolvingBehavior" )
    double probabilityOfMapped = 0.5;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UAddMappedDecoratorToCompositeMutator* mappedMutator;
    
    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UAddGeneratedDecoratorToCompositeMutator* generatedMutator;
	
public:
    UAddRandomDecoratorToCompositeMutator();
    
    virtual void Init( UServiceInjector* serviceInjector ) override;
    
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
