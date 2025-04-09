#include "BTEvolutionManagerComponent.h"
#include "EvolvingBehavior.h"

#include "BTChromosome.h"
#include "BTTemplateCollection.h"
#include "BehaviorTreeToGeneConverter.h"
#include "FitnessTracker.h"
#include "MutatingReproducer.h"
#include "PopulationManager.h"
#include "RandomGen.h"
#include "ServiceInjector.h"
#include "BTGeneratedTemplateLibrary.h"
#include "BTChromosome.h"
#include "Reproducer.h"
#include "TournamentSelector.h"
#include "TopNFitnessSelector.h"
#include "Trial.h"
#include "LifeCycleActor.h"
#include "NodeMapper.h"
#include <ctime>
#include "Kismet/KismetSystemLibrary.h"

#include <string>
#include <iostream>
#include <cstdlib>

UBTEvolutionManagerComponent::UBTEvolutionManagerComponent() : Super()
{
    rand = CreateDefaultSubobject<URandomGen>(TEXT("Rand"));
    nodeMapper = CreateDefaultSubobject<UNodeMapper>(TEXT("NodeMapper"));
    behaviorTreeConverter = CreateDefaultSubobject<UBehaviorTreeToGeneConverter>(TEXT("TreeConverter"));
    elitismReproducer = CreateDefaultSubobject<UReproducer>(TEXT("ElitismReproducer"));
    topNFitnessSelector = CreateDefaultSubobject<UTopNFitnessSelector>(TEXT("TopNFitnessSelector"));
}

void UBTEvolutionManagerComponent::Init(UServiceInjector* injector) {
    serviceInjector = injector;
    
    rand->Init(serviceInjector);
    rand->Seed(static_cast<unsigned int>(time(NULL)));
    rand->RegisterLifeCycle();

    nodeMapper->Init(serviceInjector);

    if (nullptr != templateLibrary)
    {
        templateLibrary->Init(serviceInjector);
    }
    else
    {
        UE_LOG(LogEvolvingBehaviorError, Error, TEXT("In the EvolutionManagerComponent there is no generated behavior tree template library specified - you should set this in the editor."));
        UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
    }

    behaviorTreeConverter->Init(serviceInjector);

    // Don't use the default population name without extra numbers, because we need to save separate objects in the save file by
    // differentiating them on the population name.
    if (GetPopulationName() == TEXT("Default"))
    {
        SetPopulationName(FString::Printf(TEXT("%s"), *GetName()));
    }

    UPopulationManager* popManager = NewObject<UPopulationManager>(this, UPopulationManager::StaticClass(), GetNameForSubobject(TEXT("PopulationManager")));
    UFitnessTracker* fitnessTracker = NewObject<UFitnessTracker>(this, UFitnessTracker::StaticClass(), GetNameForSubobject(TEXT("FitnessTracker")));
    UChromosomeToBTConverter* converter = NewObject<UChromosomeToBTConverter>(this, UChromosomeToBTConverter::StaticClass(), GetNameForSubobject(TEXT("ChromosomeToBTConverter")));
    popManager->SetTracker(fitnessTracker);
    popManager->SetTrialSize(TrialSize);
    popManager->Init(serviceInjector);
    fitnessTracker->Init(serviceInjector);
    fitnessTracker->SetFitnessCalculator(fitnessCalculator);
    converter->Init(serviceInjector);
    if (templateCollection == nullptr)
    {
        UE_LOG(LogEvolvingBehaviorError, Error, TEXT("In the EvolutionManagerComponent there is no template collection specified. You should set this in the editor, and add at least one Behavior Tree template to it. The template collection needs to contain the Behavior Tree(s) that you want to start the evolution process with."));
        UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
    }
    else
    {
        templateCollection->VerifyTemplates();
        templateCollection->MapTemplates(nodeMapper);
    }

    if (reproducer)
    {
        reproducer->Init(serviceInjector);
    }

    if (initialPopulationReproducer)
    {
        initialPopulationReproducer->Init(serviceInjector);
    }

    popManager->SetEvoManager(this);

}

FName UBTEvolutionManagerComponent::GetNameForSubobject(const FString& subObjectID)
{
    return FName(FString::Printf(TEXT("%s-%s"), *GetPopulationName(), *subObjectID));
}

TArray<UBTChromosome*> UBTEvolutionManagerComponent::Reproduce( TArray<UBTChromosome*> parents, UReproducer* useReproducer )
{
    ensure( rand != nullptr );
    
	if ( useReproducer == nullptr )
	{
        if ( reproducer == nullptr )
        {
            TArray<UBTChromosome*> emptyResult;
            UE_LOG(LogEvolvingBehaviorError, Error, TEXT("In the EvolutionManagerComponent (on the EvolutionControlActor), there is no Reproducer specified. You should set a Reproducer in the EvolutionManagerComponent's properties in the editor."));
            return emptyResult;
        }
        
        useReproducer = reproducer;
	}

    return useReproducer->Reproduce( parents );
}

void UBTEvolutionManagerComponent::OnRegister()
{
	Super::OnRegister();

#if WITH_EDITOR
	if (GetWorld())
	{
		ALifeCycleActor* lifeCycle = ALifeCycleActor::GetLifeCycleActor(GetWorld());
		lifeCycle->OnDeleteSave().AddUObject(this, &UBTEvolutionManagerComponent::DeleteSave);
	}
#endif
}

TArray<UBTChromosome*> UBTEvolutionManagerComponent::Reproduce( UPopulationManager* populationManager )
{
    ensure( nullptr != populationManager );

    TArray<FParentFitnessInfo> allParents = CompileFitnesses( populationManager );

    //LogFitnesses( populationManager, allParents );

	int32 numElitismChildren = FMath::Floor(ElitismFraction * allParents.Num());

    int failureStreak = 0;
    TArray<UBTChromosome*> children;
    while( children.Num() < TrialSize )
	{
        if ( failureStreak > ReproductionFailureLimit )
        {
            // If we failed to get valid parents or children too many times in a row, stop.
            UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Either Parent Selector or Reproducer failed to produce new population members. Check if Parent Selector is not properly selecting parents, or Reproducer is not producing children!"));
            break;
        }

		TArray<UBTChromosome*> parents;

		// If we are using elitism and haven't yet copied the most-fit members of the previous generation,
		// do that now.
		if (children.Num() < numElitismChildren)
		{
			topNFitnessSelector->Init(float(numElitismChildren - children.Num())/(float(allParents.Num())));
			parents = DetermineParents(populationManager, allParents, topNFitnessSelector);
		}
		else
		{
			// Otherwise, use our standard parent-determining scheme.
			parents = DetermineParents(populationManager, allParents);
		}

        if ( parents.Num() == 0 )
        {
            failureStreak++;
            continue;
        }

		TArray<UBTChromosome*> childrenThisIteration;
	
		// If we are using elitism and haven't yet copied the previous best fitness parents,
		// copy them without any mutation.
		if (children.Num() < numElitismChildren)
		{
			childrenThisIteration = Reproduce(parents, elitismReproducer);
		}
		else
		{
			childrenThisIteration = Reproduce(parents);
		}

        if ( childrenThisIteration.Num() == 0 )
        {
            failureStreak++;
            continue;
        }
        
        failureStreak = 0;

	    children.Append( childrenThisIteration );
	}

    return children;
}

TArray<FParentFitnessInfo> UBTEvolutionManagerComponent::CompileFitnesses( UPopulationManager* populationManager )
{
    ensure( nullptr != populationManager );
    TArray<FParentFitnessInfo> allParents;

    UFitnessTracker* fitnessTracker = populationManager->GetTracker();
    ensure( nullptr != fitnessTracker );

    int32 trialNum = populationManager->GetCurrentTrialNumber();
    UTrial* trial = populationManager->GetTrial( trialNum );

    TArray<int32> allIDs;
    trial->GetAllIDs( allIDs );

	for (int i = 0; i < allIDs.Num(); ++i)
	{
		int32 id = allIDs[i];
		FParentFitnessInfo fitnessInfo;
		fitnessInfo.fitness = fitnessTracker->GetFitness(id);
		fitnessInfo.id = id;

		allParents.Add(fitnessInfo);
	}

    return allParents;
}

TArray<UBTChromosome*> UBTEvolutionManagerComponent::DetermineParents( UPopulationManager* populationManager,
    TArray<FParentFitnessInfo> allParents, UParentSelector* selector )
{
	if (nullptr == selector)
	{
		// If not provided a separate selector, use our default parent selector.
		selector = parentSelector;
	}

    TArray<UBTChromosome*> result;
    if ( nullptr == populationManager)
    {
        UE_LOG(LogEvolvingBehaviorError, Error, TEXT("No population manager available while determining parents!"));
        return result;
    }

    if ( allParents.Num() == 0 )
    {
        UE_LOG(LogEvolvingBehaviorError, Error, TEXT("No parents available to select from! Is the Population empty?"));
        return result;
    }

    TArray<FParentFitnessInfo> parentSelection = selector->SelectParents( allParents, rand );

    int32 trialNum = populationManager->GetCurrentTrialNumber();
    UTrial* trial = populationManager->GetTrial( trialNum );

    for( int i = 0; i < parentSelection.Num(); ++i )
	{
	    UBTChromosome* chromosome = trial->Get( parentSelection[i].id );

	    result.Add( chromosome );
	}

    return result;
}

void UBTEvolutionManagerComponent::TestReproduction( int numGenerations )
{
    UBehaviorTree* behaviorTree = templateCollection->GetInitialTemplate( 0 );
    UBehaviorTreeToGeneConverter* converter
        = NewObject<UBehaviorTreeToGeneConverter>( this, UBehaviorTreeToGeneConverter::StaticClass(), GetNameForSubobject(TEXT("BehaviorTreeToGeneConverter")) );
    converter->Init( serviceInjector );
    UBTChromosome* chromosome = converter->Convert( this, behaviorTree );
    if( nullptr == chromosome )
	{
	    UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Error: no chromosome after conversion!" ) );
	}

    TArray<UBTChromosome*> chromosomes;
    chromosomes.Add( chromosome );
    for( int i = 0; i < numGenerations; ++i )
	{
	    TArray<FParentFitnessInfo> parentFitness;
	    for( int c = 0; c < chromosomes.Num(); ++c )
		{
		    int newIdx = parentFitness.AddDefaulted();
		    parentFitness[newIdx].id = c;
		    parentFitness[newIdx].fitness = rand->UniformIntInRange( 0, 100 );
		}

	    TArray<FParentFitnessInfo> parentSelections = parentSelector->SelectParents( parentFitness, rand );

	    TArray<UBTChromosome*> parents;
	    for( int c = 0; c < parentSelections.Num(); ++c )
		{
		    parents.Add( chromosomes[parentSelections[c].id] );
		}

	    TArray<UBTChromosome*> newChromosomes = Reproduce( parents );
	    chromosomes.Append( newChromosomes );
	}

    UE_LOG( LogEvolvingBehavior, Display, TEXT( "Results: %s" ), *chromosomes.Last()->ToString() );
}

TArray<UBTChromosome*> UBTEvolutionManagerComponent::BuildInitialPopulation()
{   
    TArray<UBTChromosome*> result;


    if (templateCollection->GetNumInitialTemplates() == 0)
    {
        UE_LOG(LogEvolvingBehaviorError, Error, TEXT("The EvolutionManagerComponent contains a template collection without any Behavior Tree templates, so no bots are being spawned. The template collection needs to contain the Behavior Tree(s) that you want to start the evolution process with." ));
        UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
        return result;
    }

    TArray<UBTChromosome*> templates;
    
    ConvertTemplates( templates );
    
    for( int i = 0; i < TrialSize; ++i )
    {
        int id = rand->UniformIntInRange( 0, templates.Num() - 1 );
        UBTChromosome* chromosome = DuplicateObject(templates[id], this);
        result.Add( chromosome );
    }
	
	for ( int gen = 0; gen < numInitialPopulationGenerations; ++gen )
	{
		result = Reproduce( result, initialPopulationReproducer );
	}

    return result;
}

void UBTEvolutionManagerComponent::ConvertTemplates( TArray<UBTChromosome*>& result )
{
    ensure( templateCollection != nullptr );
    ensure( templateCollection->GetNumInitialTemplates() > 0 );
    
    int numInitialTemplates = templateCollection->GetNumInitialTemplates();
    for( int i = 0; i < numInitialTemplates; ++i )
    {
        UBehaviorTree* behaviorTree = templateCollection->GetInitialTemplate( i );
        UBTChromosome* chromosome = behaviorTreeConverter->Convert( this, behaviorTree );
        chromosome->RegisterInitialBTAsset(behaviorTree->GetPathName());
        result.Add( chromosome );
    }
}

UBehaviorTree* UBTEvolutionManagerComponent::GetInitialBT(UBTChromosome* chromosome)
{
    UBehaviorTree* behaviorTree = templateCollection->GetInitialTemplateByID(chromosome->GetInitialBTAsset());
    
    // If we can't load the object from the templates, try loading it from disk.
    if ( nullptr == behaviorTree )
    {
        behaviorTree = LoadObject<UBehaviorTree>(nullptr, *chromosome->GetInitialBTAsset());
    }

    return behaviorTree;
}

void UBTEvolutionManagerComponent::LogFitnesses( UPopulationManager* populationManager,
    TArray<FParentFitnessInfo> allParents )
{
    ensure( nullptr != populationManager );

    int32 trialNum = populationManager->GetCurrentTrialNumber();

    UE_LOG( LogEvolvingBehavior, Display, TEXT( "Trial %i" ), trialNum );

    UTrial* trial = populationManager->GetTrial( trialNum );

    double highestFitness = std::numeric_limits<double>::min();

    for( int i = 0; i < allParents.Num(); ++i )
	{
	    UBTChromosome* chromosome = trial->Get( allParents[i].id );

	    if( allParents[i].fitness > highestFitness )
		{
		    highestFitness = allParents[i].fitness;
		}

	    UE_LOG(
	        LogEvolvingBehavior, Display, TEXT( "Chromosome %i: %s" ), allParents[i].id, *chromosome->ToString() );

	    UE_LOG( LogEvolvingBehavior, Display, TEXT( "Fitness %i: %f" ), allParents[i].id, allParents[i].fitness );
	}

    UE_LOG( LogEvolvingBehavior, Display, TEXT( "Highest fitness: %f" ), highestFitness );
}

void UBTEvolutionManagerComponent::DeleteSave()
{
	UPopulationManager* popManager = NewObject<UPopulationManager>(this, UPopulationManager::StaticClass(), GetNameForSubobject(TEXT("PopulationManager")));
	popManager->DeleteSave();
}