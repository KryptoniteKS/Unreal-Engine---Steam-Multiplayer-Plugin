// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPauseMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerSessionsSubsystem.h"
#include "SteamFriends.h"


bool UPlayerPauseMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
	}

	BindButtonCallbacks();

	return true;
}

void UPlayerPauseMenu::BindButtonCallbacks()
{
	if (Btn_Resume)
	{
		Btn_Resume->OnClicked.AddDynamic(this, &ThisClass::OnResumeButtonClicked);
	}

	if (Btn_Options)
	{
		Btn_Options->OnClicked.AddDynamic(this, &ThisClass::OnOptionsButtonClicked);
	}

	if (Btn_Invite)
	{
		Btn_Invite->OnClicked.AddDynamic(this, &ThisClass::OnInviteButtonClicked);
	}

	if (Btn_MainMenu)
	{
		Btn_MainMenu->OnClicked.AddDynamic(this, &ThisClass::OnMainMenuButtonClicked);
	}
}

void UPlayerPauseMenu::OnResumeButtonClicked()
{
	// TODO: Broadcast delegate back to player that the game has unpaused.
	OnResumeButtonClickedDelegate.Broadcast();

	RemoveFromParent();
}

void UPlayerPauseMenu::OnOptionsButtonClicked()
{
	// TODO: Create options menu
}

void UPlayerPauseMenu::OnInviteButtonClicked()
{
	MultiplayerSessionsSubsystem->ActivateInviteOverlay();
}

void UPlayerPauseMenu::OnMainMenuButtonClicked()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UWorld* ResolvedWorld = MainMenuMap.LoadSynchronous();
		if (ResolvedWorld)
		{
			MultiplayerSessionsSubsystem->LeaveCurrentLobby();
			UGameplayStatics::OpenLevelBySoftObjectPtr(World, MainMenuMap);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("The main menu map was not properly defined in the Pause Menu blueprint, so the call to return to the main menu did not sucessfully perform its function! The player will remain in the level as well as the Steam Lobby as a result."));
		}

	}
}
