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

	for (auto Result : SessionResults)
	{
		FString GameMode;
		FString LobbyName;
		FString MapName;
		bool FoundMatchingSession = true;
		Result.Session.SessionSettings.Get(FName("GameMode"), GameMode); // Gets the GameMode of the sessions and outputs it to GameMode
		Result.Session.SessionSettings.Get(FName("LobbyName"), LobbyName); // Gets the LobbyName of the sessions and outputs it to LobbyName
		Result.Session.SessionSettings.Get(FName("MapName"), MapName); // Gets the MapName of the sessions and outputs it to MapName
		MapName = MenuHelper::FormatMapName(MapName, true);

		if (Combo_GameModes->GetSelectedOption() != MenuHelper::GetAllOption() && Combo_GameModes->GetSelectedOption() != GameMode)
		{
			FoundMatchingSession = false;
		}

		if (Combo_Maps->GetSelectedOption() != MenuHelper::GetAllOption() && Combo_Maps->GetSelectedOption() != MapName)
		{
			FoundMatchingSession = false;
		}

		FString SearchText = TextSearch_Lobby->GetText().ToString().TrimStartAndEnd();
		if (!SearchText.IsEmpty() && !LobbyName.Contains(SearchText, ESearchCase::IgnoreCase))
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

	// If we have not joined a session after looping through all the session results, re-enable the Join button
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
	// TODO
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

	auto Session = SelectedSession->GetSessionSearchResult();
	if (Session.IsValid() && MultiplayerSessionsSubsystem != nullptr)
	{
		MultiplayerSessionsSubsystem->JoinSession(Session);
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Magenta,
			FString::Format(TEXT("Joining {0}'s lobby..."), { Session.Session.OwningUserName })
		);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Red,
			FString(TEXT("Either the session was invalid or the multiplayer subsystem was invalid."))
		);
	}
}

void ULobbyMenu::SearchButtonClicked()
{
	/* Call FindSessions on the MultiplayerSessionsSubsystem. */
	SearchButton->SetIsEnabled(false);
	if (MultiplayerSessionsSubsystem)
	{
		MultiplayerSessionsSubsystem->FindSessions(10000);
	}
}

void ULobbyMenu::BackButtonClicked()
{
	OnBackButtonClickedDelegate.Broadcast();
}
