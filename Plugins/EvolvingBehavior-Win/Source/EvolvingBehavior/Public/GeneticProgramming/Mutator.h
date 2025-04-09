#pragma once

#include "Mutator.generated.h"

class UServiceInjector;
class UNodeMapper;
class UBTGeneratedTemplateLibrary;
class URandomGen;
class UBTChromosome;

/**
 * @file Mutator.h
 */

/**
 * @class UMutator
 * @author npc
 * @date 04/12/17
 * @brief An abstract base class for a Mutator that changes a Chromosome in some way.
 * 
 * Given a chromosome, its parents, and the index of its primary parent,
 * a Mutator will modify the chromosome.
 * 
 * This is the abstract base class for all Mutators.
 */
UCLASS( abstract, Blueprintable, EditInlineNew, BlueprintType, DefaultToInstanced, CollapseCategories )
class EVOLVINGBEHAVIOR_API UMutator : public UObject
{
    GENERATED_BODY()

protected:
    UPROPERTY()
    UServiceInjector* serviceInjector;

public:
	virtual void Init( UServiceInjector* injector )
	{
        serviceInjector = injector;
	}

    virtual UBTChromosome* Mutate( UBTChromosome* chromosome, TArray<UBTChromosome*> parents, int primaryParent )
        PURE_VIRTUAL( UMutator::Mutate(), return chromosome; );
};
