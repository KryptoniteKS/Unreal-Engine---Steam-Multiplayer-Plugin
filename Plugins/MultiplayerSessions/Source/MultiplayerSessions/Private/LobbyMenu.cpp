// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMenu.h"
#include "Components/Button.h"
#include "SessionEntry.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystem.h"
#include "MenuHelper.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "Components/CheckBox.h"
#include "SteamMatchmaking.h"

bool ULobbyMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (JoinButton)
	{
		// Bind our JoinButtonClicked function to the button click event
		JoinButton->OnClicked.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}
	if (SearchButton)
	{
		SearchButton->OnClicked.AddDynamic(this, &ThisClass::SearchButtonClicked);
	}
	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &ThisClass::BackButtonClicked);
	}

	FillMapComboBox();
	FillGameModeComboBox();

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem)
		{
			MultiplayerSessionsSubsystem->MultiplayerOnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessions);
			MultiplayerSessionsSubsystem->OnRequestLobbyListComplete.AddDynamic(this, &ThisClass::OnRequestLobbyList);
			MultiplayerSessionsSubsystem->OnJoinLobbyComplete.AddDynamic(this, &ThisClass::OnJoinLobby);
			//MultiplayerSessionsSubsystem->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::OnJoinSession);
		}
	}

	return true;
}


void ULobbyMenu::FillMapComboBox()
{
	if (Combo_Maps)
	{
		MenuHelper::PopulateMapComboBox(Combo_Maps, true);
	}
}

void ULobbyMenu::FillGameModeComboBox()
{
	if (Combo_GameModes)
	{
		MenuHelper::PopulateGameModeComboBox(Combo_GameModes, true);
	}
}

void ULobbyMenu::OnRequestLobbyList(const TArray<FLobbyEntry>& LobbyData)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		15.f,
		FColor::Red,
		FString(TEXT("Lobby Data Received! Creating widget..."))
	);
	UE_LOG(LogTemp, Error, TEXT("Lobby Data has been received by our Lobby Menu. Creating widgets for %d entries..."), LobbyData.Num());


	for (auto LobbyEntry : LobbyData)
	{
		USessionEntry* NewSession = CreateWidget<USessionEntry>(this, SessionEntryClass);
		NewSession->SetLobbyEntry(LobbyEntry);
		NewSession->LobbyEntrySetup();
		NewSession->GetSessionEntryButton()->SetStyle(NormalButtonStyle);

		NewSession->OnSessionSelectedDelegate.AddDynamic(this, &ThisClass::OnSessionEntrySelected);

		UE_LOG(LogTemp, Error, TEXT("Adding entry to the menu with Host Name %s, Map Name %s, and Game Mode %s."), *LobbyEntry.HostName, *LobbyEntry.MapName, *LobbyEntry.MapName);
		AddSession(NewSession);
	}

	// After looping through all the results, re-enable the Search button
	SearchButton->SetIsEnabled(true);
}

void ULobbyMenu::OnJoinLobby(bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Red,
			FString(TEXT("Failed to join the Steam lobby!"))
		);

		return;
	}

	GEngine->AddOnScreenDebugMessage(
		-1,
		15.f,
		FColor::Emerald,
		FString(TEXT("Successfully joined the Steam lobby! Attempting to join affiliated listen server..."))
	);

	MultiplayerSessionsSubsystem->JoinCurrentLobbyListenServer();

}

void ULobbyMenu::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	if (MultiplayerSessionsSubsystem == nullptr)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(
		-1,
		15.f,
		FColor::Red,
		FString(TEXT("Finding sessions..."))
	);

	FString SelectedGameMode = Combo_GameModes->GetSelectedOption();
	FString SelectedLobbyName = TextSearch_Lobby->GetText().ToString().TrimStartAndEnd();
	FString SelectedMapName = MenuHelper::FormatMapName(Combo_Maps->GetSelectedOption(), true);
	bool ShowFullLobbies = Check_ShowFullLobbies->GetCheckedState() == ECheckBoxState::Checked;
	FString AllOption = MenuHelper::GetAllOption();

	for (auto Result : SessionResults)
	{
		FString ServerGameMode;
		FString ServerLobbyName;
		FString ServerMapName;
		int32 maxPlayers = Result.Session.SessionSettings.NumPublicConnections;
		int32 numPlayers = maxPlayers - Result.Session.NumOpenPublicConnections;

		bool FoundMatchingSession = true;

		Result.Session.SessionSettings.Get(FName("GameMode"), ServerGameMode); // Gets the GameMode of the sessions and outputs it to ServerGameMode
		Result.Session.SessionSettings.Get(FName("LobbyName"), ServerLobbyName); // Gets the LobbyName of the sessions and outputs it to ServerLobbyName
		Result.Session.SessionSettings.Get(FName("MapName"), ServerMapName); // Gets the MapName of the sessions and outputs it to ServerMapName

		if (!SelectedGameMode.Equals(AllOption) && !SelectedGameMode.Equals(ServerGameMode))
		{
			FoundMatchingSession = false;
		}

		if (!SelectedMapName.Equals(AllOption) && !SelectedMapName.Equals(ServerMapName))
		{
			FoundMatchingSession = false;
		}

		if (!ShowFullLobbies && numPlayers >= maxPlayers)
		{
			FoundMatchingSession = false;
		}

		if (!SelectedLobbyName.IsEmpty() && !ServerLobbyName.Contains(SelectedLobbyName, ESearchCase::IgnoreCase))
		{
			FoundMatchingSession = false;
		}
		
		if (FoundMatchingSession)
		{
			// If we have found a session that matches our filters, let's add it to the list.
			USessionEntry* NewSession = CreateWidget<USessionEntry>(this, SessionEntryClass);
			NewSession->SetSessionSearchResult(Result);
			NewSession->SessionEntrySetup();
			NewSession->GetSessionEntryButton()->SetStyle(NormalButtonStyle);
			
			NewSession->OnSessionSelectedDelegate.AddDynamic(this, &ThisClass::OnSessionEntrySelected);
			AddSession(NewSession);
		}
	}

	// After looping through all the results, re-enable the Search button
	SearchButton->SetIsEnabled(true);
}

void ULobbyMenu::AddSession(USessionEntry* Session)
{
	// Must perform a cast for this method to work properly even though USessionEntry is a UUserWidget
	ScrollBox_Sessions->AddChild(Cast<UWidget>(Session));
	Sessions.Add(Session);
}

void ULobbyMenu::ClearSessions()
{
	ScrollBox_Sessions->ClearChildren();
}

// Callback function for when a session entry button is clicked
void ULobbyMenu::OnSessionEntrySelected(USessionEntry* Session)
{
	if (Session)
	{
		if (SelectedSession)
		{
			SelectedSession->GetSessionEntryButton()->SetStyle(NormalButtonStyle);
		}
		
		Session->GetSessionEntryButton()->SetStyle(SelectedButtonStyle);
		SelectedSession = Session;
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Invalid session selected."))
			);
		}
	}
}

void ULobbyMenu::JoinButtonClicked()
{
	if (!SelectedSession)
	{
		return;
	}

	auto LobbyEntry = SelectedSession->GetLobbyEntry();
	auto LobbyId = LobbyEntry.LobbyID;
	MultiplayerSessionsSubsystem->JoinLobby(LobbyId);


	//if (!SelectedSession)
	//{
	//	return;
	//}

	//auto Session = SelectedSession->GetSessionSearchResult();
	//if (Session.IsValid() && MultiplayerSessionsSubsystem != nullptr)
	//{
	//	MultiplayerSessionsSubsystem->JoinSession(Session);
	//	GEngine->AddOnScreenDebugMessage(
	//		-1,
	//		15.f,
	//		FColor::Magenta,
	//		FString::Format(TEXT("Joining {0}'s lobby..."), { Session.Session.OwningUserName })
	//	);
	//}
	//else
	//{
	//	GEngine->AddOnScreenDebugMessage(
	//		-1,
	//		15.f,
	//		FColor::Red,
	//		FString(TEXT("Either the session was invalid or the multiplayer subsystem was invalid."))
	//	);
	//}
}

void ULobbyMenu::SearchButtonClicked()
{
	SearchButton->SetIsEnabled(false);
	ClearSessions();
	if (MultiplayerSessionsSubsystem)
	{
		// Add our filters, if they exist
		auto SelectedMap = Combo_Maps->GetSelectedOption(); // formatted map name
		auto SelectedGameMode = Combo_GameModes->GetSelectedOption();
		auto AllOption = MenuHelper::GetAllOption();
		// If we are not filtering by ALL maps, filter by the specific map
		if (!SelectedMap.Equals(AllOption))
		{
			auto MapNameKey = MultiplayerSessionsSubsystem->GetMapNameKey();
			USteamMatchmaking::AddRequestLobbyListStringFilter(MapNameKey, SelectedMap, ESteamLobbyComparison::LobbyComparisonEqualTo);
		}
		// If we are not filtering by ALL game modes, filter by the specific game mode
		if (!SelectedGameMode.Equals(AllOption))
		{
			auto GameModeKey = MultiplayerSessionsSubsystem->GetGameModeKey();
			USteamMatchmaking::AddRequestLobbyListStringFilter(GameModeKey, SelectedGameMode, ESteamLobbyComparison::LobbyComparisonEqualTo);
		}
		// This will eventually trigger our callback on this class: OnFindSessions()
		//MultiplayerSessionsSubsystem->FindSessions(1000);
		MultiplayerSessionsSubsystem->RequestLobbyList();
	}
}

void ULobbyMenu::BackButtonClicked()
{
	OnBackButtonClickedDelegate.Broadcast();
}
