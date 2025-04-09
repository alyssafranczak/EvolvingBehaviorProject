
#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Representation/BTChromosome.h"
#include "GeneticProgramming/ParentSelector.h"
#include "Representation/ServiceInjector.h"
#include "RandomGen.h"

#include "BTEvolutionManagerComponent.generated.h"

class UBTTemplateCollection;
class UBTGeneratedTemplateLibrary;
class URandomGen;
class UServiceInjector;
class UNodeMapper;
class UBlackboardData;
class UReproducer;
class UServiceInjector;
class UPopulationManager;
class UBehaviorTreeToGeneConverter;
class UFitnessCalculator;

class UParentSelector;
class UBehaviorTree;
class UTopNFitnessSelector;

/**
 * @file BTEvolutionManagerComponent.H
 */

/**
 * @class UBTEvolutionManagerComponent
 * @author npc
 * @date 12/12/17
 * @brief Manages the settings for evolution.
 */
UCLASS( ClassGroup = ( Custom ), meta = ( BlueprintSpawnableComponent ) )
class EVOLVINGBEHAVIOR_API UBTEvolutionManagerComponent : public UActorComponent
{
    GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Saving", meta = (ClampMin = "0.00", ClampMax = "100.00"))
	float PercentOfTreesToSave = 100.0;

    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UBTTemplateCollection* templateCollection;

    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UReproducer* reproducer;

	// Reproducer that exactly copies the parent, for elitism.
	UPROPERTY( Instanced )
	UReproducer* elitismReproducer;

	// Fitness selector that deterministically takes the top-N most fit, for elitism.
	UPROPERTY( Instanced )
	UTopNFitnessSelector* topNFitnessSelector;

    UPROPERTY(EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UFitnessCalculator* fitnessCalculator;

    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UBTGeneratedTemplateLibrary* templateLibrary;

    UPROPERTY( EditAnywhere, Instanced, Category = "EvolvingBehavior")
    UParentSelector* parentSelector;
	
    UPROPERTY( EditAnywhere, Instanced, Category = "InitialPopulation" )
    UReproducer* initialPopulationReproducer;
	
	UPROPERTY( EditAnywhere, Category="InitialPopulation")
	int32 numInitialPopulationGenerations;

	UPROPERTY(EditAnywhere, Instanced, Category = "Optional")
	URandomGen* rand;

    UPROPERTY( EditAnywhere, Category = "Advanced")
    int ReproductionFailureLimit = 100;

	UPROPERTY(Instanced)
	UNodeMapper* nodeMapper;

	UPROPERTY(Instanced)
	UBehaviorTreeToGeneConverter* behaviorTreeConverter;

	UPROPERTY(Instanced)
	UServiceInjector* serviceInjector;

    UPROPERTY(EditAnywhere, Category = "EvolvingBehavior")
    int32 TrialSize = 10;

	/** The fraction (0 to 1) of best-fitness individuals to copy unmodified into the next generation. */
	UPROPERTY(EditAnywhere, meta = (ClampMin = "0.00", ClampMax = "1.00"), Category = "EvolvingBehavior")
	float ElitismFraction = 0.2f;

    UPROPERTY(EditAnywhere, Category = "EvolvingBehavior")
    FString PopulationName = "Default";
    
public:

    UBTEvolutionManagerComponent();
    
    void Init(UServiceInjector* injector);

    void SetPopulationName( FString name ) { PopulationName = name; }

    virtual TArray<UBTChromosome*> Reproduce( TArray<UBTChromosome*> parents, UReproducer* useReproducer = nullptr );

	void OnRegister();

    virtual TArray<UBTChromosome*> Reproduce( UPopulationManager* populationManager );

    virtual TArray<UBTChromosome*> BuildInitialPopulation();
	
    virtual UBehaviorTree* GetInitialBT(UBTChromosome* chromosome);

    FString GetPopulationName() { return PopulationName; }

	UReproducer* GetReproducer()
	{
		return reproducer;
	}

	float GetPercentOfTreesToSave() { return PercentOfTreesToSave; }

protected:

    void TestReproduction( int numGenerations );

    void ConvertTemplates( TArray<UBTChromosome*>& result );
    
    TArray<FParentFitnessInfo> CompileFitnesses( UPopulationManager* populationManager );

    TArray<UBTChromosome*> DetermineParents( UPopulationManager* populationManager, TArray<FParentFitnessInfo> allParents, UParentSelector* selector = nullptr );
	
	void LogFitnesses( UPopulationManager* populationManager, TArray<FParentFitnessInfo> parentFitnesses );

	void DeleteSave();

    FName GetNameForSubobject(const FString& subObjectID);
};
