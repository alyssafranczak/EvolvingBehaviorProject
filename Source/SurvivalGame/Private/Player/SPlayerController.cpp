// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.


#include "Player/SPlayerController.h"
#include "Player/SPlayerCameraManager.h"
#include "Player/SCharacter.h"
#include "SurvivalGame/STypes.h"
#include "UI/SHUD.h"
#include "World/SGameState.h"

/* Define a log category for error messages */
DEFINE_LOG_CATEGORY_STATIC(LogGame, Log, All);


ASPlayerController::ASPlayerController(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
	/* Assign the class types we wish to use */
	PlayerCameraManagerClass = ASPlayerCameraManager::StaticClass();

	/* Example - Can be set to true for debugging, generally a value like this would exist in the GameMode instead */
	bRespawnImmediately = false;
}


void ASPlayerController::UnFreeze()
{
	Super::UnFreeze();

	// Check if match is ending or has ended.
	ASGameState* MyGameState = GetWorld()->GetGameState<ASGameState>();
	if (MyGameState && MyGameState->HasMatchEnded())
	{
		/* Don't allow spectating or respawns */
		return;
	}

	/* Respawn or spectate */
	if (bRespawnImmediately)
	{
		ServerRestartPlayer();
		if (PlayerState->IsSpectator())
		{
			StopSpectating();
		}
	}
	else
	{
		StartSpectating();
	}
}


void ASPlayerController::StartSpectating()
{
	/* Update the state on server */
	PlayerState->SetIsSpectator(true);
	/* Waiting to respawn */
	bPlayerIsWaiting = true;
	ChangeState(NAME_Spectating);
	/* Push the state update to the client */
	ClientGotoState(NAME_Spectating);

	/* Focus on the remaining alive player */
	ViewAPlayer(1);

	/* Update the HUD to show the spectator screen */
	ClientHUDStateChanged(EHUDState::Spectating);
}

void ASPlayerController::StopSpectating()
{
	/* Update the state on server */
	PlayerState->SetIsSpectator(false);
	/* Waiting to respawn */
	bPlayerIsWaiting = false;
	ChangeState(NAME_Playing);
	/* Push the state update to the client */
	ClientGotoState(NAME_Playing);

	/* Update the HUD to show the spectator screen */
	ClientHUDStateChanged(EHUDState::Playing);
}


void ASPlayerController::Suicide()
{
	if (IsInState(NAME_Playing))
	{
		ServerSuicide();
	}
}

void ASPlayerController::ServerSuicide_Implementation()
{
	ASCharacter* MyPawn = Cast<ASCharacter>(GetPawn());
	if (MyPawn && ((GetWorld()->TimeSeconds - MyPawn->CreationTime > 1) || (GetNetMode() == NM_Standalone)))
	{
		MyPawn->Suicide();
	}
}


bool ASPlayerController::ServerSuicide_Validate()
{
	return true;
}


void ASPlayerController::ClientHUDStateChanged_Implementation(EHUDState NewState)
{
	ASHUD* HUD = Cast<ASHUD>(GetHUD());
	if (HUD)
	{
		HUD->OnStateChanged(NewState);
	}
}


void ASPlayerController::ClientHUDMessage_Implementation(EHUDMessage MessageID)
{
	/* Turn the ID into a message for the HUD to display */
	FText TextMessage = GetText(MessageID);

	ASHUD* HUD = Cast<ASHUD>(GetHUD());
	if (HUD)
	{
		/* Implemented in SurvivalHUD Blueprint */
		HUD->MessageReceived(TextMessage);
	}
}

/* Temporarily set the namespace. If this was omitted, we should call NSLOCTEXT(Namespace, x, y) instead */
#define LOCTEXT_NAMESPACE "HUDMESSAGES"

FText ASPlayerController::GetText(EHUDMessage MsgID)
{
	switch (MsgID)
	{
	case EHUDMessage::Weapon_SlotTaken:
		return LOCTEXT("WeaponSlotTaken", "Weapon slot already taken.");
	case EHUDMessage::Character_EnergyRestored:
		return LOCTEXT("CharacterEnergyRestored", "Energy Restored");
	case EHUDMessage::Game_SurviveStart:
		return LOCTEXT("GameNightStart", "SURVIVE THE NIGHT");
	case EHUDMessage::Game_SurviveEnded:
		return LOCTEXT("GameNightEnd", "Night survived! Prepare for the coming night.");
	default:
		UE_LOG(LogGame, Warning, TEXT("No Message set for enum value in SPlayerContoller::GetText(). "))
		return FText::FromString("No Message Set");
	}
}

/* Remove the namespace definition so it doesn't exist in other files compiled after this one. */
#undef LOCTEXT_NAMESPACE