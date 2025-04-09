// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "EvolutionControlActor.h"
#include "EvolvingBehavior.h"

#include "GameFramework/GameState.h"
#include "Engine/World.h"
#include "LifeCycleActor.h"
#include "Engine/Engine.h"
#include "PopulationManager.h"

#if WITH_EDITOR
#include "Editor.h"
#include "Editor/EditorEngine.h"
#endif

// Sets default values
AEvolutionControlActor::AEvolutionControlActor(const class FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these
	// features off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;

	// ...
	serviceInjector = CreateDefaultSubobject<UServiceInjector>(TEXT("Service Injector"));

	if (nullptr == evolutionManagerComponent)
	{
		evolutionManagerComponent = CreateDefaultSubobject<UBTEvolutionManagerComponent>(TEXT("UBTEvolutionManagerComponent"));
	}

}

#if WITH_EDITOR
void AEvolutionControlActor::PostActorCreated()
{
	UWorld* world = GetWorld();
	
	if ( world && !ALifeCycleActor::GetLifeCycleActor(world) )
	{
		// If there is no LifeCycle Actor in the map, add one.
		GEditor->AddActor(world->GetCurrentLevel(), ALifeCycleActor::StaticClass(), FTransform(FVector(0)));
	}
}
#endif

void AEvolutionControlActor::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	evolutionManagerComponent->RegisterComponent();

	evolutionManagerComponent->Init(serviceInjector);

}

void AEvolutionControlActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UPopulationManager* popMan = Cast<UPopulationManager>(serviceInjector->GetService(UPopulationManager::StaticClass()));
	popMan->RegisterLifeCycle();
}

// Called every frame
void AEvolutionControlActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

int AEvolutionControlActor::SecondsRemaining()
{
	return ALifeCycleActor::GetLifeCycleActor(GetWorld())->GetSecondsRemaining();
}

AEvolutionControlActor* AEvolutionControlActor::GetEvolutionControlActor(UObject* worldContextObj, const FString& name)
{
	UWorld* world = GEngine->GetWorldFromContextObject(worldContextObj, EGetWorldErrorMode::LogAndReturnNull);
	if (nullptr == world)
	{
		return nullptr;
	}

	TActorIterator<AEvolutionControlActor> ActorItr(world, AEvolutionControlActor::StaticClass(), EActorIteratorFlags::SkipPendingKill);
	for (; ActorItr; ++ActorItr)
	{
		AEvolutionControlActor* actor = *ActorItr;

		if (actor->GetHumanReadableName().Equals(name, ESearchCase::IgnoreCase))
		{
			return actor;
		}
	}

	return nullptr;
}

void AEvolutionControlActor::GetEvolutionControlActors(UObject* worldContextObj, TArray<AEvolutionControlActor*>& outActors)
{
	UWorld* world = GEngine->GetWorldFromContextObject(worldContextObj, EGetWorldErrorMode::LogAndReturnNull);
	if (nullptr == world)
	{
		return;
	}

	TActorIterator<AEvolutionControlActor> ActorItr(world, AEvolutionControlActor::StaticClass(), EActorIteratorFlags::SkipPendingKill);
	for (; ActorItr; ++ActorItr)
	{
		AEvolutionControlActor* actor = *ActorItr;

		outActors.Add(actor);
	}
}

void AEvolutionControlActor::GetEvolutionControlActorsByTag(UObject* worldContextObj, const FName& tag, TArray<AEvolutionControlActor*>& outActors)
{
	UWorld* world = GEngine->GetWorldFromContextObject(worldContextObj, EGetWorldErrorMode::LogAndReturnNull);
	if (nullptr == world)
	{
		return;
	}

	TActorIterator<AEvolutionControlActor> ActorItr(world, AEvolutionControlActor::StaticClass(), EActorIteratorFlags::SkipPendingKill);
	for (; ActorItr; ++ActorItr)
	{
		AEvolutionControlActor* actor = *ActorItr;
		
		if (actor->ActorHasTag(tag)) 
		{
			outActors.Add(actor);
		}
	}
}

UPopulationManager* AEvolutionControlActor::GetPopulationManager()
{
	UPopulationManager* popMan = serviceInjector->GetService<UPopulationManager>();
	return popMan;
}

UFitnessTracker* AEvolutionControlActor::GetFitnessTracker()
{
	UFitnessTracker* fitnessTracker = serviceInjector->GetService<UFitnessTracker>();
	return fitnessTracker;
}

void AEvolutionControlActor::RegisterFitnessUpdater( TScriptInterface<IFitnessUpdater> fitnessUpdater)
{
	UFitnessTracker* fitnessTracker = GetFitnessTracker();
	fitnessTracker->RegisterFitnessUpdater(fitnessUpdater);
}
	
bool AEvolutionControlActor::TryRetrieveNextPopMember(FPopulationReg& outRegistration, const FNewTrialPopulationReadyHandler& popReadyEvent)
{
	UPopulationManager* popManager = GetPopulationManager();

	if ( !popManager->IsTrialReady() || !popManager->Register(outRegistration) )
	{
		if (popManager->OnTrialPopulationReady().Contains(popReadyEvent))
		{
			return false;
		}

		popManager->OnTrialPopulationReady().Add(popReadyEvent);
		return false;
	}

	return true;
}