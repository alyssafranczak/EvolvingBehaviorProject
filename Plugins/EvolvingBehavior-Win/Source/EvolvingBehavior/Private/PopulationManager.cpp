#include "PopulationManager.h"
#include "ServiceInjector.h"
#include "EvolvingBehavior.h"
#include "LifeCycleActor.h"

#include "BTNodeGene.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Save/SaveEvolvingBehavior.h"
#include "Save/BehaviorTreeAssetSaver.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Kismet/GameplayStatics.h"
#include "HAL/PlatformFilemanager.h"

void UPopulationManager::SetTrialSize(int32 ts)
{
    TrialSize = ts;
}

void UPopulationManager::RegisterLifeCycle()
{
    ALifeCycleActor* lc = ALifeCycleActor::GetLifeCycleActor(GetWorld());
    lc->OnGenerateInitialPopulation().AddUObject(this, &UPopulationManager::GenerateInitialPopulation);
    lc->OnNextTrial().AddUObject(this, &UPopulationManager::Breed);
    lc->OnSave().AddUObject(this, &UPopulationManager::Save);
    lc->OnLoad().AddUObject(this, &UPopulationManager::Load);
    lc->OnSaveTrees().AddUObject(this, &UPopulationManager::SaveTrees);
    lc->OnLogTrialResults().AddUObject(this, &UPopulationManager::LogTrialResults);

	// TODO: Do we want some configuration switch to turn on/off this fitness log saving?
	lc->OnLogTrialResults().AddUObject(this, &UPopulationManager::SaveFitnessResultLog);

	lc->OnResetExperiment().AddUObject(this, &UPopulationManager::ResetExperiment);
}

void UPopulationManager::ResetExperiment()
{
	loadedPopulations.Empty();
	tracker->Reset();
	lastRegisteredID = 0;
}

void UPopulationManager::Save(USaveEvolvingBehavior* sg)
{
    sg->SaveObject(this);
}

void UPopulationManager::Load(USaveEvolvingBehavior* sg)
{
    sg->LoadObject(this);
}

void UPopulationManager::DeleteSave()
{
	int populationIndex = 0;
	FString saveSlotName = GetSaveSlotName();

	while (UGameplayStatics::DoesSaveGameExist(saveSlotName, populationIndex))
	{
		UGameplayStatics::DeleteGameInSlot(saveSlotName, populationIndex);
		populationIndex++;
	}
}

void UPopulationManager::GenerateInitialPopulation()
{
	currentTrialNum = 0;
    TArray<UBTChromosome*> initialPop = emc->BuildInitialPopulation();

    UE_LOG( LogEvolvingBehavior, Display, TEXT( "Initial Population Size: %i" ), initialPop.Num() );

    UTrial* NewTrial = NewObject<UTrial>( this, GetTrialObjectName(currentTrialNum) );
    loadedPopulations.Add(currentTrialNum, NewTrial);

    PopulateTrial( 1, initialPop.Num()+1, NewTrial, initialPop );
    
    if( IsTrialReady() )
    {
        BroadcastNewTrialPopulationReady();
    }

    return;
}

void UPopulationManager::SaveTrees(int32 trialNumber) {
    UChromosomeToBTConverter* converter = serviceInjector->GetService<UChromosomeToBTConverter>();
    UFitnessTracker* fitnessTracker = serviceInjector->GetService<UFitnessTracker>();

    UTrial* trial = GetCurrTrial();
    TArray<int32> ids;
    trial->GetAllIDs(ids);

	ALifeCycleActor* lc = ALifeCycleActor::GetLifeCycleActor(GetWorld());
	FString experimentID = lc->GetExperimentID();

	float percentToSave = emc->GetPercentOfTreesToSave();
	int numTreesToSave = FMath::CeilToInt(ids.Num() * percentToSave / 100.0);

	// Sort the IDs by fitness.
	ids.Sort([&](const int32 a, const int32 b)
	{
		return fitnessTracker->GetFitness(a) > fitnessTracker->GetFitness(b);
	});

    for (int idx = 0; idx < numTreesToSave; idx++) {
        int32 id = ids[idx];
        UBTChromosome* chromosome = trial->Get(id);

        UBehaviorTree* originalBT = emc->GetInitialBT(chromosome);

        // get behavior tree
        UBehaviorTree* newTree = converter->Convert(this, originalBT, chromosome);

        if (nullptr == newTree)
        {
            UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Error: no tree converted! Chromosome: %s"),
                *chromosome->ToString());
            return;
        }

        int32 fitness = fitnessTracker->GetFitness(id);

        const FString fileName = FString::Printf(TEXT("%sTree-ID%i-Fit%i"), *(emc->GetPopulationName()), id, fitness);
        const FString filePath = FPaths::Combine(TreeSavePath, FString::Printf(TEXT("/%s/Trial%i"), *experimentID, trialNumber));
        UBehaviorTreeAssetSaver::SaveTree(newTree, fileName, filePath);
    }

}

void UPopulationManager::PopulateTrial( int first, int last, UTrial* trial, TArray<UBTChromosome*> trialPopulation )
{
    for( int i = first; i < last; i++ )
    {
        if( trialPopulation.Num() == 0 )
        {
            // We ran out of members in the population.
            break;
        }
        
        UBTChromosome* chromosome = trialPopulation.Pop();
        chromosome->Rename( NULL, trial );
        trial->Add( i, chromosome );
    }

    if( IsTrialReady() )
    {
        BroadcastNewTrialPopulationReady();
    }
}

void UPopulationManager::LogTrialResults(int32 trialNumber)
{
    UTrial* trial = GetTrial(trialNumber);
    TArray<int32> ids;
    check(trial != nullptr);
    trial->GetAllIDs(ids);

    double avgFitness = 0;

    for (int i = 0; i < ids.Num(); i++)
    {
        int32 currId = ids[i];
        double score = tracker->GetFitness(currId);

		// TODO: Make logging individual chromosomes and their fitnesses optional?
        /*UBTChromosome* chromosome = trial->Get(currId);
		UE_LOG(LogEvolvingBehavior, Display, TEXT("ID: %i"), currId);

        if (chromosome != nullptr)
        {
            chromosome->ToLog();
			UE_LOG(LogEvolvingBehavior, Display, TEXT("Score: %f"), score);			
        }*/

        avgFitness += score / ids.Num();
    }

    UE_LOG(LogEvolvingBehavior, Display, TEXT("Average Fitness for trial %d: %f"), trialNumber,
        avgFitness);
}

void UPopulationManager::SaveFitnessResultLog(int32 trialNum)
{
	UTrial* trial = GetTrial(trialNum);

	TArray<int32> allIDs;
	trial->GetAllIDs(allIDs);

	ALifeCycleActor* lc = ALifeCycleActor::GetLifeCycleActor(GetWorld());
	FString experimentID = lc->GetExperimentID();

	FString ConfigPath = FPaths::ProjectSavedDir();
	FString FilePath = FPaths::Combine(*ConfigPath, TEXT("EvolvingBehavior-FitnessStats"), *experimentID, *(emc->GetPopulationName() + TEXT("FitnessLog.csv")));

	// If fitness log file doesn't exist, create it
	FString FileContent = TEXT("");
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	if (!PlatformFile.FileExists(*FilePath))
	{
		FFileHelper::SaveStringToFile(FileContent, *FilePath);
	}

	for (int i = 0; i < allIDs.Num(); ++i)
	{
		int32 id = allIDs[i];

		FileContent = FStringOutputDevice::SanitizeFloat(tracker->GetFitness(id));
		if (i != allIDs.Num() - 1)
		{
			FileContent = FileContent + TEXT(",");
		}

		FFileHelper::SaveStringToFile(FileContent, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);
	}

	FileContent = TEXT("\n");
	FFileHelper::SaveStringToFile(FileContent, *FilePath, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), EFileWrite::FILEWRITE_Append);

}

void UPopulationManager::SetEvoManager( UBTEvolutionManagerComponent* em )
{
    emc = TWeakObjectPtr<UBTEvolutionManagerComponent>( em );
}

UFitnessTracker* UPopulationManager::GetTracker() { return tracker; }

void UPopulationManager::SetTracker( UFitnessTracker* t ) { tracker = t; }

void UPopulationManager::Breed(int32 trialNumber)
{
    if (loadedPopulations.Contains( trialNumber ))
	{
        UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Tried to breed to a trial that already exists"));
        return;
    }

    TArray<UBTChromosome*> chromosomes = emc->Reproduce( this );
    if ( chromosomes.Num() == 0 )
    {
        UE_LOG(LogEvolvingBehaviorError, Error, TEXT("We did not get any children created for the next generation! Look for error messages above, there may be a misconfiguration of the Reproducer."));
        UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
    }

    UTrial* NewTrial = NewObject<UTrial>( this, GetTrialObjectName(trialNumber) );
	loadedPopulations.Add(trialNumber, NewTrial);
    PopulateTrial( lastRegisteredID+1, ( lastRegisteredID + 1 + TrialSize ), NewTrial, chromosomes );

	currentTrialNum = trialNumber;

	BroadcastNewTrialPopulationReady();
}

UTrial* UPopulationManager::GetTrial( int32 trial )
{
    if( trial > GetCurrentTrialNumber() )
    {
        return nullptr;
    }

	if (!IsTrialNumReady(trial))
	{
		LoadTrial(trial);
	}

    return loadedPopulations[trial];
}

bool UPopulationManager::AsyncLoadTrial(int32 trial)
{
	if (IsTrialNumReady(trial))
	{
		// Don't double-load.
		return true;
	}

	FString slotName = GetSaveSlotName();
	if (!UGameplayStatics::DoesSaveGameExist(slotName, trial))
	{
		// If we have no save game, don't bother trying to load.
		UE_LOG(LogEvolvingBehaviorError, Error, TEXT("No trial available to load for num: %d"), trial);
		return false;
	}

	// Set up the delegate.
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	// USomeUObjectClass::LoadGameDelegateFunction is a void function that takes the following parameters: const FString& SlotName, const int32 UserIndex, USaveGame* LoadedGameData
	LoadedDelegate.BindUObject(this, &UPopulationManager::OnLoadedTrial);

	UGameplayStatics::AsyncLoadGameFromSlot(slotName, trial, LoadedDelegate);
	return true;
}


bool UPopulationManager::LoadTrial(int32 trial)
{
	FString slotName = GetSaveSlotName();
	if (!UGameplayStatics::DoesSaveGameExist(slotName, trial))
	{
		// If we have no save game, don't bother trying to load.
		UE_LOG(LogEvolvingBehaviorError, Error, TEXT("No trial available to load for num: %s"), trial);
		return false;
	}

	OnLoadedTrial(slotName, trial, UGameplayStatics::LoadGameFromSlot(slotName, trial));
	return true;
}

FString UPopulationManager::GetSaveSlotName()
{
	ALifeCycleActor* lifeCycle = ALifeCycleActor::GetLifeCycleActor(GetWorld());
	if (nullptr == lifeCycle)
	{
		// If we have no save game, don't bother trying to load.
		UE_LOG(LogEvolvingBehaviorError, Error, TEXT("No lifecycle actor found when looking for save info."));
		return FString(TEXT(""));
	}
	return FString::Printf(TEXT("%s%s"), *lifeCycle->GetSaveSlotName(), *GetName());
}

void UPopulationManager::OnLoadedTrial(const FString& slot, const int32 userIdx, USaveGame* sg)
{
	if (sg == nullptr)
	{
		return;
	}
	USaveEvolvingBehavior* saveData = (Cast<USaveEvolvingBehavior>(sg));
	if (nullptr == saveData)
	{
		UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Error: Save data was not in correct format."));
		return;
	}

	UTrial* trial = NewObject<UTrial>(this, GetTrialObjectName(slot, userIdx));
	if (!saveData->LoadObject(trial))
	{
		UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Error: Save data did not contain trial object %d"), userIdx);
		return;
	}
	loadedPopulations.Add(userIdx, trial);

	trial->SetServiceInjector(serviceInjector);

	if (userIdx == GetCurrentTrialNumber())
	{
		BroadcastNewTrialPopulationReady();
	}
}

FName UPopulationManager::GetTrialObjectName(int32 trialNum)
{
	FString slotName = GetSaveSlotName();
	return GetTrialObjectName(slotName, trialNum);
}

FName UPopulationManager::GetTrialObjectName(FString saveSlotName, int32 trialNum)
{
	return FName(FString::Printf(TEXT("%s%d"), *saveSlotName, trialNum));
}

void UPopulationManager::AsyncSave()
{
	FString saveSlotName = GetSaveSlotName();
	for (TPair<int32, UTrial*> trialInfo : loadedPopulations)
	{
		if (USaveEvolvingBehavior* SaveGameInstance = Cast<USaveEvolvingBehavior>(UGameplayStatics::CreateSaveGameObject(USaveEvolvingBehavior::StaticClass())))
		{
			// Set up the (optional) delegate.
			FAsyncSaveGameToSlotDelegate SavedDelegate;

			//USomeUObjectClass::SaveGameDelegateFunction is a void function that takes the following parameters: const FString& SlotName, const int32 UserIndex, bool bSuccess
			SavedDelegate.BindUObject(this, &UPopulationManager::OnSaveFinished);

			SaveGameInstance->SaveObject(trialInfo.Value);

			// Start async save process.
			UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, saveSlotName, trialInfo.Key, SavedDelegate);
		}
	}
}

void UPopulationManager::Save()
{
	FString saveSlotName = GetSaveSlotName();
	for (TPair<int32, UTrial*> trialInfo : loadedPopulations)
	{
		if (USaveEvolvingBehavior* SaveGameInstance = Cast<USaveEvolvingBehavior>(UGameplayStatics::CreateSaveGameObject(USaveEvolvingBehavior::StaticClass())))
		{
			SaveGameInstance->SaveObject(trialInfo.Value);

			// Start async save process.
			UGameplayStatics::SaveGameToSlot(SaveGameInstance, saveSlotName, trialInfo.Key);
		}
	}
}

void UPopulationManager::OnSaveFinished(const FString& slot, const int32 userIdx, bool success)
{
	if (!success)
	{
		UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Failed to save population (ID: %s)"), userIdx);
	}
}

bool UPopulationManager::Register( FPopulationReg& outReg )
{
    if( !IsTrialReady() )
    {
        UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Trial not ready." ) );
        return false;
    }

	// We want IDs to start at 1, not 0, so add 1 to the last registered so far to get the next free ID.
    int32 newTreeID = lastRegisteredID + 1;
    
    UTrial* trial = GetTrial( GetCurrentTrialNumber() );

	// If we have previously-freed IDs available, use one of those instead of a new ID.
	if (trial->GetNumAvailable() <= FreedIDs.Num())
	{
		if (FreedIDs.Num() <= 0)
		{
			return false;
		}

		newTreeID = FreedIDs.Pop();
	}

    // create registration
    UBTChromosome* chromosome = trial->Get( newTreeID );
    trial->OnSpawned( newTreeID );
    
    if( nullptr == chromosome )
    {
        UE_LOG(
               LogEvolvingBehaviorWarning, Warning, TEXT( "No chromosome available in trial: %i" ), lastRegisteredID );
        return false;
    }

    UBehaviorTree* originalBT = emc->GetInitialBT(chromosome);   

    UChromosomeToBTConverter* converter = serviceInjector->GetService<UChromosomeToBTConverter>();
    // get behavior tree
    UBehaviorTree* newTree = converter->Convert( this, originalBT, chromosome );
    
    if( nullptr == newTree )
    {
        UE_LOG( LogEvolvingBehaviorError, Error, TEXT( "Error: no tree converted! Chromosome: %s" ),
               *chromosome->ToString() );
        return false;
    }
    
    outReg.id = newTreeID;
    outReg.bt = newTree;
    
	if (!tracker->ContainsFitnessFor(outReg.id))
	{
		UpdateInitialFitness(outReg.id);
	}
    
	if (newTreeID > lastRegisteredID)
	{
		// If we used a tree ID greater than our last highest registered, update our last registered so we don't duplicate in the future.
		lastRegisteredID = newTreeID;
	}

    return true;
}

void UPopulationManager::ReleaseRegistration(int32 id)
{
	if (!IsTrialReady())
	{
		UE_LOG(LogEvolvingBehaviorError, Error, TEXT("Trial not ready."));
		return;
	}

	if (!IsRegistered(id))
	{
		return;
	}

	UTrial* trial = GetTrial(GetCurrentTrialNumber());
	trial->OnReleased(id);

	FreedIDs.Add(id);
}

bool UPopulationManager::IsRegistered(int32 id)
{
	if (id > lastRegisteredID)
	{
		return false;
	}

	if (FreedIDs.Contains(id))
	{
		return false;
	}

	return true;
}

void UPopulationManager::UpdateInitialFitness( int32 id )
{
    UTrial* trial = GetTrial( GetCurrentTrialNumber() );
    
    UBTChromosome* chromosome = trial->Get( id );
    
    UFitness* fitness = NewObject<UFitness>( chromosome, UFitness::StaticClass() );
    tracker->UpdateFitness( id, fitness );
}

int32 UPopulationManager::GetPopulationRemaining()
{
    if( !IsTrialReady() )
    {
        return 0;
    }
    int32 trialNum = GetCurrentTrialNumber();
    return GetTrial( trialNum )->GetNumAvailable();
}

int32 UPopulationManager::GetNumSpawnedThisTrial()
{
    if( !IsTrialReady() )
    {
        return 0;
    }
    int32 trialNum = GetCurrentTrialNumber();
    return GetTrial( trialNum )->GetNumSpawned();
}

bool UPopulationManager::IsTrialReady()
{
	return IsTrialNumReady(GetCurrentTrialNumber());
}

bool UPopulationManager::IsTrialNumReady(int32 trialNum)
{
	return emc.IsValid() && loadedPopulations.Contains(trialNum);
}

void UPopulationManager::DataLoaded_Implementation()
{
	AsyncLoadTrial(GetCurrentTrialNumber());

    for (auto trialInfo : loadedPopulations)
    {
        trialInfo.Value->SetServiceInjector(serviceInjector);
    }
}

void UPopulationManager::DataSaved_Implementation()
{
	Save();
}
