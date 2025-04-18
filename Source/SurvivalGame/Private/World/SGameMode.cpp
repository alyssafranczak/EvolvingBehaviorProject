// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.


#include "World/SGameMode.h"
#include "Player/SPlayerController.h"
#include "Player/SPlayerState.h"
#include "World/SGameState.h"
#include "Player/SCharacter.h"
#include "UI/SHUD.h"
#include "SurvivalGame/STypes.h"
#include "Player/SSpectatorPawn.h"
#include "AI/SZombieAIController.h"
#include "AI/SZombieCharacter.h"
#include "World/SPlayerStart.h"
#include "Mutators/SMutator.h"
#include "Items/SWeapon.h"


ASGameMode::ASGameMode(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	/* Assign the class types used by this gamemode */
	PlayerControllerClass = ASPlayerController::StaticClass();
	PlayerStateClass = ASPlayerState::StaticClass();
	GameStateClass = ASGameState::StaticClass();
	SpectatorClass = ASSpectatorPawn::StaticClass();

	bAllowFriendlyFireDamage = false;
	bSpawnZombiesAtNight = true;

	/* Start the game at 16:00 */
	TimeOfDayStart = 16 * 60;
	BotSpawnInterval = 5.0f;

	/* Default team is 1 for players and 0 for enemies */
	PlayerTeamNum = 1;
}


void ASGameMode::InitGameState()
{
	Super::InitGameState();

	ASGameState* MyGameState = Cast<ASGameState>(GameState);
	if (MyGameState)
	{
		MyGameState->ElapsedGameMinutes = TimeOfDayStart;
	}
}


void ASGameMode::PreInitializeComponents()
{
	Super::PreInitializeComponents();

	/* Set timer to run every second */
	GetWorldTimerManager().SetTimer(TimerHandle_DefaultTimer, this, &ASGameMode::DefaultTimer, GetWorldSettings()->GetEffectiveTimeDilation(), true);
}


void ASGameMode::StartMatch()
{
	if (!HasMatchStarted())
	{
		/* Spawn a new bot every 5 seconds (bothandler will opt-out based on his own rules for example to only spawn during night time) */
		GetWorldTimerManager().SetTimer(TimerHandle_BotSpawns, this, &ASGameMode::SpawnBotHandler, BotSpawnInterval, true);
	}

	Super::StartMatch();
}


void ASGameMode::DefaultTimer()
{
	/* Immediately start the match while playing in editor */
	//if (GetWorld()->IsPlayInEditor())
	{
		if (GetMatchState() == MatchState::WaitingToStart)
		{
			StartMatch();
		}
	}

	/* Only increment time of day while game is active */
	if (IsMatchInProgress())
	{
		ASGameState* MyGameState = Cast<ASGameState>(GameState);
		if (MyGameState)
		{
			/* Increment our time of day */
			MyGameState->ElapsedGameMinutes += MyGameState->GetTimeOfDayIncrement();

			/* Determine our state */
			MyGameState->GetAndUpdateIsNight();

			/* Trigger events when night starts or ends */
			bool CurrentIsNight = MyGameState->GetIsNight();
			if (CurrentIsNight != LastIsNight)
			{
				EHUDMessage MessageID = CurrentIsNight ? EHUDMessage::Game_SurviveStart : EHUDMessage::Game_SurviveEnded;
				MyGameState->BroadcastGameMessage(MessageID);

				/* The night just ended, respawn all dead players */
				if (!CurrentIsNight)
				{
					OnNightEnded();
				}

				/* Update bot states */
				if (CurrentIsNight)
				{
					WakeAllBots();
				}
				else
				{
					PassifyAllBots();
				}
			}

			LastIsNight = MyGameState->bIsNight;
		}
	}
}


bool ASGameMode::CanDealDamage(ASBaseCharacter* InstigatorPawn, ASBaseCharacter* DamagedPawn, class ASPlayerState* DamageCauser, class ASPlayerState* DamagedPlayer) const
{
	if (bAllowFriendlyFireDamage)
	{
		return true;
	}

	/* Allow damage to self */
	if (DamagedPlayer == DamageCauser)
	{
		return true;
	}

	// Compare Team Numbers
	return DamageCauser && DamagedPlayer && 
		((DamageCauser->GetTeamNumber() != DamagedPlayer->GetTeamNumber()) ||
		 (InstigatorPawn && DamagedPawn && InstigatorPawn->GetTeam() != DamagedPawn->GetTeam()));
}


FString ASGameMode::InitNewPlayer(class APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	FString Result = Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);

	ASPlayerState* NewPlayerState = Cast<ASPlayerState>(NewPlayerController->PlayerState);
	if (NewPlayerState)
	{
		NewPlayerState->SetTeamNumber(PlayerTeamNum);
	}

	return Result;
}


float ASGameMode::ModifyDamage(float Damage, AActor* DamagedActor, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) const
{
	float ActualDamage = Damage;

	ASBaseCharacter* DamagedPawn = Cast<ASBaseCharacter>(DamagedActor);
	ASBaseCharacter* InstigatorPawn = Cast<ASBaseCharacter>(DamageCauser);
	if (DamagedPawn && EventInstigator)
	{
		ASPlayerState* DamagedPlayerState = DamagedPawn->GetPlayerState<ASPlayerState>();
		ASPlayerState* InstigatorPlayerState = EventInstigator->GetPlayerState<ASPlayerState>();

		// Check for friendly fire
		if (!CanDealDamage(InstigatorPawn, DamagedPawn, InstigatorPlayerState, DamagedPlayerState))
		{
			ActualDamage = 0.f;
		}
	}

	return ActualDamage;
}


bool ASGameMode::ShouldSpawnAtStartSpot(AController* Player)
{
	/* Always pick a random location */
	return false;
}


AActor* ASGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> PreferredSpawns;
	TArray<APlayerStart*> FallbackSpawns;

	/* Get all playerstart objects in level */
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);

	/* Split the player starts into two arrays for preferred and fallback spawns */
	for (int32 i = 0; i < PlayerStarts.Num(); i++)
	{
		APlayerStart* TestStart = Cast<APlayerStart>(PlayerStarts[i]);

		if (TestStart && IsSpawnpointAllowed(TestStart, Player))
		{
			if (IsSpawnpointPreferred(TestStart, Player))
			{
				PreferredSpawns.Add(TestStart);
			}
			else
			{
				FallbackSpawns.Add(TestStart);
			}
		}

	}

	/* Pick a random spawnpoint from the filtered spawn points */
	APlayerStart* BestStart = nullptr;
	if (PreferredSpawns.Num() > 0)
	{
		BestStart = PreferredSpawns[FMath::RandHelper(PreferredSpawns.Num())];
	}
	else if (FallbackSpawns.Num() > 0)
	{
		BestStart = FallbackSpawns[FMath::RandHelper(FallbackSpawns.Num())];
	}

	/* If we failed to find any (so BestStart is nullptr) fall back to the base code */
	return BestStart ? BestStart : Super::ChoosePlayerStart_Implementation(Player);
}


bool ASGameMode::IsSpawnpointAllowed(APlayerStart* SpawnPoint, AController* Controller)
{
	if (Controller == nullptr || Controller->PlayerState == nullptr)
		return true;

	/* Check for extended playerstart class */
	ASPlayerStart* MyPlayerStart = Cast<ASPlayerStart>(SpawnPoint);
	if (MyPlayerStart)
	{
		return MyPlayerStart->GetIsPlayerOnly() && !Controller->PlayerState->IsABot();
	}

	/* Check if controller pawn's tag matches the tag of the PlayerStart*/
	// return Controller->ActorHasTag(SpawnPoint->PlayerStartTag);

	if (Controller->GetPawn()) {
		UE_LOG(LogTemp, Warning, TEXT("Pawn class: %s"), *Controller->GetPawn()->GetClass()->GetName());
		if (Controller->GetPawn()->ActorHasTag(SpawnPoint->PlayerStartTag)) {
			return true;
		}
		else {
			return false;
		}
	}


	///* Cast failed, Anyone can spawn at the base playerstart class */
	return true;
}


bool ASGameMode::IsSpawnpointPreferred(APlayerStart* SpawnPoint, AController* Controller)
{
	if (SpawnPoint)
	{
		/* Iterate all pawns to check for collision overlaps with the spawn point */
		const FVector SpawnLocation = SpawnPoint->GetActorLocation();
		for (TActorIterator<APawn> It(GetWorld()); It; ++It)
		{
			ACharacter* OtherPawn = Cast<ACharacter>(*It);
			if (OtherPawn)
			{
				const float CombinedHeight = (SpawnPoint->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + OtherPawn->GetCapsuleComponent()->GetScaledCapsuleHalfHeight()) * 2.0f;
				const float CombinedWidth = SpawnPoint->GetCapsuleComponent()->GetScaledCapsuleRadius() + OtherPawn->GetCapsuleComponent()->GetScaledCapsuleRadius();
				const FVector OtherLocation = OtherPawn->GetActorLocation();

				// Check if player overlaps the playerstart
				if (FMath::Abs(SpawnLocation.Z - OtherLocation.Z) < CombinedHeight && (SpawnLocation - OtherLocation).Size2D() < CombinedWidth)
				{
					return false;
				}
			}
		}

		/* Check if spawnpoint is exclusive to players */
		ASPlayerStart* MyPlayerStart = Cast<ASPlayerStart>(SpawnPoint);
		if (MyPlayerStart)
		{
			return MyPlayerStart->GetIsPlayerOnly() && !Controller->PlayerState->IsABot();
		}
	}

	return false;
}


void ASGameMode::SpawnNewBot(TSubclassOf<APawn> botPawnType )
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APawn* pawn = GetWorld()->SpawnActor<APawn>(botPawnType, SpawnInfo);
	pawn->SpawnDefaultController();

	AController* controller = pawn->GetController();
	AActor* startPoint = FindPlayerStart(controller);
	if (nullptr != startPoint)
	{
		pawn->TeleportTo(startPoint->GetActorLocation(), startPoint->GetActorRotation());
	}
}

/* Used by RestartPlayer() to determine the pawn to create and possess when a bot or player spawns */
UClass* ASGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (Cast<ASZombieAIController>(InController) && BotPawnTypes.Num() > 0 )
	{
		return BotPawnTypes[0];
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}


bool ASGameMode::CanSpectate_Implementation(APlayerController* Viewer, APlayerState* ViewTarget)
{
	/* Don't allow spectating of other non-player bots */
	return (ViewTarget && !ViewTarget->IsABot());
}


void ASGameMode::PassifyAllBots()
{
	for (TActorIterator<APawn> It(GetWorld()); It; ++It)
	{
		ASZombieCharacter* AIPawn = Cast<ASZombieCharacter>(*It);
		if (AIPawn)
		{
			AIPawn->SetBotType(EBotBehaviorType::Passive);
		}
	}
}


void ASGameMode::WakeAllBots()
{
	for (TActorIterator<APawn> It(GetWorld()); It; ++It)
	{
		ASZombieCharacter* AIPawn = Cast<ASZombieCharacter>(*It);
		if (AIPawn)
		{
			AIPawn->SetBotType(EBotBehaviorType::Patrolling);
		}
	}
}


void ASGameMode::SpawnBotHandler_Implementation()
{
	if (!bSpawnZombiesAtNight)
		return;

	ASGameState* MyGameState = Cast<ASGameState>(GameState);
	if (MyGameState)
	{
		/* Only spawn bots during night time */
		if (MyGameState->GetIsNight())
		{
			/* This could be any dynamic number based on difficulty (eg. increasing after having survived a few nights) */
			const int32 MaxEnemies = 10;
				
			if ( BotPawnTypes.Num() < 1 )
			{
				UE_LOG(LogSurvivalGame, Warning, TEXT("Not spawning a bot - no bot types provided in GameMode list of bot types!"))
				return;
			}

			/* Check number of available pawns */
			if (GetNumBotsAlive() < MaxEnemies)
			{
				SpawnNewBot(BotPawnTypes[FMath::RandRange(0, BotPawnTypes.Num()-1)]);
			}
		}
	}
}

int ASGameMode::GetNumBotsAlive()
{
	int numBotsAlive = 0;
	for (TActorIterator<ASZombieAIController> botIter(GetWorld()); botIter; ++botIter )
	{
		++numBotsAlive;
	}

	return numBotsAlive;
}

void ASGameMode::OnNightEnded()
{
	// Do nothing (can be used to apply score or trigger other time of day events)
}

void ASGameMode::Killed(AController* Killer, AController* VictimPlayer, APawn* VictimPawn, const UDamageType* DamageType)
{
	// Do nothing (can we used to apply score or keep track of kill count)
}


void ASGameMode::SetPlayerDefaults(APawn* PlayerPawn)
{
	Super::SetPlayerDefaults(PlayerPawn);

	SpawnDefaultInventory(PlayerPawn);
}


void ASGameMode::SpawnDefaultInventory(APawn* PlayerPawn)
{
	ASCharacter* MyPawn = Cast<ASCharacter>(PlayerPawn);
	if (MyPawn)
	{
		for (int32 i = 0; i < DefaultInventoryClasses.Num(); i++)
		{
			if (DefaultInventoryClasses[i])
			{
				FActorSpawnParameters SpawnInfo;
				SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				ASWeapon* NewWeapon = GetWorld()->SpawnActor<ASWeapon>(DefaultInventoryClasses[i], SpawnInfo);

				MyPawn->AddWeapon(NewWeapon);
			}
		}
	}
}


/************************************************************************/
/* Modding & Mutators                                                   */
/************************************************************************/


void ASGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	// HACK: workaround to inject CheckRelevance() into the BeginPlay sequence
	UFunction* Func = AActor::GetClass()->FindFunctionByName(FName(TEXT("ReceiveBeginPlay")));
	Func->FunctionFlags |= FUNC_Native;
	Func->SetNativeFunc(&ASGameMode::BeginPlayMutatorHack);
	
	/* Spawn all mutators. */
	for (int32 i = 0; i < MutatorClasses.Num(); i++)
	{
		AddMutator(MutatorClasses[i]);
	}

	if (BaseMutator)
	{
		BaseMutator->InitGame(MapName, Options, ErrorMessage);
	}

	Super::InitGame(MapName, Options, ErrorMessage);
}


bool ASGameMode::CheckRelevance_Implementation(AActor* Other)
{
	/* Execute the first in the mutator chain */
	if (BaseMutator)
	{
		return BaseMutator->CheckRelevance(Other);
	}

	return true;
}

void ASGameMode::BeginPlayMutatorHack(UObject* Context, FFrame& Stack, RESULT_DECL)
{
    P_FINISH;

    // WARNING: This function is called by every Actor in the level during his BeginPlay sequence. Meaning:  'this' is actually an AActor! Only do AActor things!
    if (!P_THIS->IsA(ALevelScriptActor::StaticClass()) && !P_THIS->IsA(ASMutator::StaticClass()) &&
        (P_THIS->RootComponent == NULL || P_THIS->RootComponent->Mobility != EComponentMobility::Static || (!P_THIS->IsA(AStaticMeshActor::StaticClass()) && !P_THIS->IsA(ALight::StaticClass()))))
    {
        ASGameMode* Game = P_THIS->GetWorld()->GetAuthGameMode<ASGameMode>();
        // a few type checks being AFTER the CheckRelevance() call is intentional; want mutators to be able to modify, but not outright destroy
        if (Game != NULL && Game != P_THIS && !Game->CheckRelevance((AActor*)P_THIS) && !P_THIS->IsA(APlayerController::StaticClass()))
        {
            /* Actors are destroyed if they fail the relevance checks (which moves through the gamemode specific check AND the chain of mutators) */
            P_THIS->Destroy();
        }
    }
}
/*
void ASGameMode::BeginPlayMutatorHack(FFrame& Stack, RESULT_DECL)
{
	P_FINISH;

	// WARNING: This function is called by every Actor in the level during his BeginPlay sequence. Meaning:  'this' is actually an AActor! Only do AActor things!
	if (!IsA(ALevelScriptActor::StaticClass()) && !IsA(ASMutator::StaticClass()) &&
		(RootComponent == NULL || RootComponent->Mobility != EComponentMobility::Static || (!IsA(AStaticMeshActor::StaticClass()) && !IsA(ALight::StaticClass()))))
	{
		ASGameMode* Game = GetWorld()->GetAuthGameMode<ASGameMode>();
		// a few type checks being AFTER the CheckRelevance() call is intentional; want mutators to be able to modify, but not outright destroy
		if (Game != NULL && Game != this && !Game->CheckRelevance((AActor*)this) && !IsA(APlayerController::StaticClass()))
		{
			// Actors are destroyed if they fail the relevance checks (which moves through the gamemode specific check AND the chain of mutators)
			Destroy();
		}
	}
}
*/

void ASGameMode::AddMutator(TSubclassOf<ASMutator> MutClass)
{
	ASMutator* NewMut = GetWorld()->SpawnActor<ASMutator>(MutClass);
	if (NewMut)
	{
		if (BaseMutator == nullptr)
		{
			BaseMutator = NewMut;
		}
		else
		{
			// Add as child in chain
			BaseMutator->NextMutator = NewMut;
		}
	}
}
