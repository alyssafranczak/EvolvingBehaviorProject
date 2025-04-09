
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"
#include "BTCompositeNodeGene.h"
#include "RemoveMappedCompositeMutator.generated.h"

/**
 * @file RemoveCompositeMutator.h
 */

/**
 * @class URemoveMappedCompositeMutator
 * @author jim
 * @date 3/24/19
 * @brief Removes a random mapped composite node from the chromosome if one exists.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API URemoveMappedCompositeMutator : public UMutator
{
    GENERATED_BODY()
	
public:
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
private:
    void CopyData( UBTCompositeNodeGene* nodeToRemove, UBTCompositeNodeGene* parentNode );
};
