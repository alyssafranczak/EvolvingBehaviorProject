
#pragma once

#include "CoreMinimal.h"
#include "Reproducer.h"
#include "MutatingReproducer.generated.h"

class UMutator;
class UNodeMapper;
class UBTGeneratedTemplateLibrary;

/**
 * @file MutatingReproducer.h
 */

/**
 * @class FMutatorInfo
 * @author npc
 * @date 12/04/17
 * @brief A reproducer that randomly mutates its reproduced nodes at some defined probabilities.
 * 
 * The MutatingReproducer takes a set of Mutators and probabilities of using them.
 * 
 * It then generates new children, and applies mutators with the given probabilities.
 */
USTRUCT()
struct FMutatorInfo
{
	GENERATED_BODY()
	
	UPROPERTY( EditAnywhere, Category = "EvolvingBehavior")
	float probability; 
	
	UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
	UMutator* mutator;
};

UCLASS()
class EVOLVINGBEHAVIOR_API UMutatingReproducer : public UReproducer
{
	GENERATED_BODY()
	
private:
	UPROPERTY( EditAnywhere, Category = "EvolvingBehavior")
	TArray<FMutatorInfo> mutators;

protected:

public:

	void Init( TArray<FMutatorInfo> newMutators, UServiceInjector* serviceInjector );

	virtual void Init( UServiceInjector* serviceInjector ) override;

	virtual TArray<UBTChromosome*> Reproduce( TArray<UBTChromosome*> parents );

	virtual TArray<FMutatorInfo>& GetMutators()
	{
		return mutators;
	}
    
};
