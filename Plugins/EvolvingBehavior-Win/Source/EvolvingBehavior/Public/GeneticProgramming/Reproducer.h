
#pragma once

#include "Reproducer.generated.h"

class UBTChromosome;
class UServiceInjector;

/**
 * @file Reproducer.h
 */

/**
 * @class UReproducer
 * @author npc
 * @date 12/12/17
 * @brief The abstract base class for reproducing chromosomes into children.
 */
UCLASS( Blueprintable, BlueprintType, EditInlineNew, DefaultToInstanced, CollapseCategories )
class EVOLVINGBEHAVIOR_API UReproducer : public UObject
{
	GENERATED_BODY()
	
private:
	
protected:

	UPROPERTY()
	UServiceInjector* ServiceInjector;


public:
	virtual void Init( UServiceInjector* serviceInjector );
	
	virtual TArray<UBTChromosome*> Reproduce( TArray<UBTChromosome*> parents );
};
