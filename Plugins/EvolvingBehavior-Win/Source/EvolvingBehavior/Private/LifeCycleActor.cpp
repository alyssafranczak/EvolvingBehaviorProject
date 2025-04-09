#include "LifeCycleActor.h"

#include "EvolvingBehavior.h"
#include "Save/SaveEvolvingBehavior.h"
#include "Save/BehaviorTreeAssetSaver.h"
#include "RandomGen.h"

#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameState.h"
#include "Engine/World.h"
#include "EvolutionControlActor.h"

// Sets default values for this component's properties
ALifeCycleActor::ALifeCycleActor()
{
	PrimaryActorTick.bCanEverTick = true;

	rand = CreateDefaultSubobject<URandomGen>(TEXT("Rand"));

	GenerateExperimentID();
}

void ALifeCycleActor::PreInitializeComponents() {
	Super::PreInitializeComponents();

    AsyncLoad();
}

// Called when the game starts
void ALifeCycleActor::BeginPlay()
{
    Super::BeginPlay();
	
    UE_LOG(LogEvolvingBehavior, Display, TEXT("Trial Length: %i, %i"), TrialLength, TrialLength)
    UE_LOG(LogEvolvingBehavior, Display, TEXT("Total Trials: %i, %i"), TotalTrials, TotalTrials)

	if ( noSaveAvailable)
	{
		// If we are not loading a save, we need to start a trial from scratch.
		StartTrial();
	}
}

void ALifeCycleActor::RestartExperiment()
{
	this->trialNumber = -1;

	waitingForTrial = true;

	OnResetExperiment().Broadcast();

	AsyncSave();
}

// Called every frame
void ALifeCycleActor::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );

    AActor* owner = GetOwner();

    // Check if the trial has finished and start the next one if so.
    CheckIfTrialTimeExpired();
}

void ALifeCycleActor::CheckIfTrialTimeExpired()
{
	if (GetSecondsRemaining() < -30)
	{
		// We have been waiting too long for the save to finish. Something went wrong - reload.
		// FIXME: It would probably be better to have a real fix, rather than a fallback.
		OnSaveFinished(SlotName, UserIndex, true);
		return;
	}
	
	if (waitingForTrial || experimentEnding) { return; }

	AGameState* gs = Cast<AGameState>(GetWorld()->GetGameState());

	if (gs == nullptr)
	{
		return;
	}

	if (GetSecondsRemaining() > 0)
	{
		return;
	}

	// Time has expired. Start the next trial.
	NextTrial();
}


void ALifeCycleActor::NextTrial()
{
	if (waitingForTrial || experimentEnding) { return; }

	LogTrialResults();

	SaveTrees();

	trialNumber++;
	if (trialNumber >= TotalTrials && TotalTrials != 0)
	{
		EndExperiment();
		return;
	}

	UE_LOG(LogEvolvingBehavior, Display, TEXT("Trial number incremented to %i"), trialNumber);

	OnNextTrial().Broadcast(trialNumber);

	AsyncSave();
}

void ALifeCycleActor::EndExperiment()
{
	if (experimentEnding)
	{
		// Already ending, don't do it twice.
		return;
	}

	experimentEnding = true;
	OnExperimentComplete().Broadcast();

	UE_LOG(LogEvolvingBehavior, Display, TEXT("Experiment completed. Loading end of experiment map."));

	// Check if our trial number is still indicating the end of the experiment (i.e., we haven't been restarted by an experiment manager)
	if (trialNumber >= TotalTrials && TotalTrials != 0)
	{
		// End experiment by opening the level specified as the post-experiment level.
		UGameplayStatics::OpenLevel(this, ExperimentEndingMap, false);
	}
}

void ALifeCycleActor::LogTrialResults()
{
	OnLogTrialResults().Broadcast(trialNumber);
}

void ALifeCycleActor::SaveTrees()
{
	OnSaveTrees().Broadcast(trialNumber);
}

void ALifeCycleActor::AsyncSave()
{
	if (USaveEvolvingBehavior* SaveGameInstance = Cast<USaveEvolvingBehavior>(UGameplayStatics::CreateSaveGameObject(USaveEvolvingBehavior::StaticClass())))
	{
		waitingForTrial = true;
		
		// Set up the (optional) delegate.
		FAsyncSaveGameToSlotDelegate SavedDelegate;

		//USomeUObjectClass::SaveGameDelegateFunction is a void function that takes the following parameters: const FString& SlotName, const int32 UserIndex, bool bSuccess
		SavedDelegate.BindUObject(this, &ALifeCycleActor::OnSaveFinished);

		// Set data on the savegame object.
		SaveGameInstance->TrialNumber = trialNumber;

		// Set the experiment ID on the savegame object.
		SaveGameInstance->ExperimentID = experimentID;

		OnSave().Broadcast(SaveGameInstance);

		// Start async save process.
		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, SlotName, UserIndex, SavedDelegate);
	}
	else
	{
		UE_LOG(LogEvolvingBehavior, Warning, TEXT("Could not create save object."));
	}
}

void ALifeCycleActor::OnSaveFinished(const FString& slot, const int32 userIdx, bool success)
{
	if (!success)
	{
		UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Failed to save. Trying to save and end trial again."));
	}
	
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()), false);
}

void ALifeCycleActor::AsyncLoad()
{
	if (!UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		// If we have no save game, don't bother trying to load, and mark that we need to start fresh.
		UE_LOG(LogEvolvingBehavior, Display, TEXT("No save data available, starting a new experiment."));
		noSaveAvailable = true;
		return;
	}

	// Set up the delegate.
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	// USomeUObjectClass::LoadGameDelegateFunction is a void function that takes the following parameters: const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData
	LoadedDelegate.BindUObject(this, &ALifeCycleActor::Load);

	UGameplayStatics::AsyncLoadGameFromSlot(SlotName, UserIndex, LoadedDelegate);

}

void ALifeCycleActor::Load(const FString& slot, const int32 userIdx, USaveGame* sg)
{
	if (sg == nullptr)
	{
		StartTrial();
		return;
	}
	USaveEvolvingBehavior* saveData = (Cast<USaveEvolvingBehavior>(sg));
	if (nullptr == saveData)
	{
		UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Error: Save data was not in correct format."));
		StartTrial();
		return;
	}

	this->trialNumber = saveData->TrialNumber;

	this->experimentID = saveData->ExperimentID;

	OnLoad().Broadcast(saveData);

	UE_LOG(LogEvolvingBehavior, Display, TEXT("Finished Loading Trial %i"), trialNumber);

	StartTrial();
}

void ALifeCycleActor::StartTrial()
{
	if (GameSpeedModifier > 0)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), GameSpeedModifier);
	}

	//If first population hasn't been generated, generate one
	if (trialNumber < 0)
	{
		GenerateInitialPopulationEvent.Broadcast();
		trialNumber = 0;
	}

	trialStartTime = GetWorld()->GetTimeSeconds();
}

float ALifeCycleActor::GetSecondsRemaining()
{
	return TrialLength - (GetWorld()->GetTimeSeconds() - trialStartTime);
}

void ALifeCycleActor::DeleteSave(const FString& slot, const int32 userIdx)
{
	UGameplayStatics::DeleteGameInSlot(slot, userIdx);

	OnDeleteSave().Broadcast();
}

void ALifeCycleActor::GenerateExperimentID()
{
    FString eID = "";

    for( int i = 0; i < 5; i++ )
    {
        int32 num = rand->UniformIntInRange( 0, 9 );
        eID.Append(FString::FromInt(num));
    }
    experimentID = eID;
}
