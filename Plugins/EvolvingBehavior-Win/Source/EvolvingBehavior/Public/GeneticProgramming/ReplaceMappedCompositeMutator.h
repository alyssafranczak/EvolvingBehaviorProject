
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "ServiceInjector.h"
#include "ReplaceMappedCompositeMutator.generated.h"

/**
 * @file ReplaceMappedCompositeMutator.h
 */

/**
 * @class UReplaceMappedCompositeMutator
 * @author npc
 * @date 12/12/17
 * @brief Replaces a random composite node in the chromosome with a random one from the NodeMapper.
 */

class UBTCompositeNodeGene;

UCLASS()
class EVOLVINGBEHAVIOR_API UReplaceMappedCompositeMutator : public UMutator
{
    GENERATED_BODY()
    
    UPROPERTY()
    UNodeMapper* mapper;

private:
    void CopyData( UBTCompositeNodeGene* oldNode, UBTCompositeNodeGene* newNode );

public:
    virtual void Init( UServiceInjector* serviceInjector ) override;
    
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
