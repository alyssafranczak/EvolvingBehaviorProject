
#pragma once

#include "CoreMinimal.h"
#include "Mutator.h"

#include "CrossoverMutator.generated.h"

class UBTChildContainerGene;

/**
 * @file CrossoverMutator.h
 */

/**
 * @class UCrossoverMutator
 * @author npc
 * @date 12/12/17
 * @brief When applied to a chromosome, this chooses a second random chromosome to 'crossover' with. 
 * A random node in each is chosen, and the node in this chromosome is replaced by the node from the selected 
 * chromosome, along with everything under it.
 * 
 * The parent with which to swap a branch is selected uniformly at random, and the branch is also
 * selected uniformly at random from each tree. Therefore, this may grow or shrink a chromosome.
 */
UCLASS()
class EVOLVINGBEHAVIOR_API UCrossoverMutator : public UMutator
{
    GENERATED_BODY()

private:
    UBTChromosome* SelectParentChromosome( TArray<UBTChromosome*> parents, int primaryParent );

    void CrossoverWithTheirTask( UBTChromosome* parentChromosome, UBTChildContainerGene* ourChildContainer );
	
    void CrossoverWithTheirComposite( UBTChromosome* parentChromosome, UBTChildContainerGene* ourChildContainer, UBTChromosome* ourChromosome );
	
	bool ShouldCrossoverTaskNode( UBTChromosome* chromosome );

public:
    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent );
};
