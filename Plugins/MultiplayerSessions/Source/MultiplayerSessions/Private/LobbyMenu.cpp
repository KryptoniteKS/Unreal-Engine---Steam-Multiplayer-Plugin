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
		Result.Session.SessionSettings.Get(FName("GameMode"), GameMode); // Gets the GameMode of the sessions and outputs it to SettingsValue
		if (GameMode == "Free For All")
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Green,
				FString(TEXT("Successfully found a session. Attempting to add it to the list..."))
			);

			// If we have found a session that matches our defined MatchType, let's add it to the list.
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
