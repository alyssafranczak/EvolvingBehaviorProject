
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"

#include "DepthBiasedCrossoverMutator.generated.h"

class UBTChildContainerGene;

/**
 * @file DepthBiasedCrossoverMutator.h
 */

/**
 * @class UDepthBiasedCrossoverMutator
 * @author jim
 * @date 5/20/19
 * @brief This performs the same function as CrossoverMutator. However the depth within the behavior tree that
 * nodes are selected from is biased towards a variable set in the editor.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UDepthBiasedCrossoverMutator : public UMutator
{
    GENERATED_BODY()

public:
    UPROPERTY( EditAnywhere, meta = (ClampMin = "0.01", ClampMax = "100"), Category = "EvolvingBehavior")
    double biasStrength = 1.5;
    
private:
    UBTChromosome* SelectParentChromosome( TArray<UBTChromosome*> parents, int primaryParent );
    
    void CrossoverWithTheirNode( UBTChildContainerGene* ourChildContainer, UBTChildContainerGene* theirChildContainer );
    
    int SelectLayer( UBTChromosome* chromosome );
public:
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
