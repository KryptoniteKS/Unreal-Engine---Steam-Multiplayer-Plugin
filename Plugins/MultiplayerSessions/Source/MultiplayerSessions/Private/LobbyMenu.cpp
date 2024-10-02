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

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Cyan,
				FString(TEXT("Successfully initialized Multiplayer Sessions Subsystem on Lobby Menu. Binding callbacks..."))
			);

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
		FString(TEXT("Test finding sessions..."))
	);

	for (auto Result : SessionResults)
	{
		FString SettingsValue;
		Result.Session.SessionSettings.Get(FName("MatchType"), SettingsValue); // Gets the MatchType of the sessions and outputs it to SettingsValue
		if (SettingsValue == "FreeForAll")
		{
			// If we have found a session that matches our defined MatchType, let's add it to the list.
			USessionEntry* NewSession = CreateWidget<USessionEntry>(this, SessionEntryClass);
			NewSession->SessionEntrySetup();
			NewSession->GetSessionEntryButton()->SetStyle(NormalButtonStyle);
			NewSession->GetPingTextBox()->SetText(FText::AsNumber(Result.PingInMs));
			NewSession->GetLobbyTextBox()->SetText(FText::FromString(Result.Session.OwningUserName));
			int32 maxPlayers = Result.Session.SessionSettings.NumPublicConnections;
			int32 numPlayers = maxPlayers - Result.Session.NumOpenPublicConnections;
			FText playersText = FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(numPlayers), FText::AsNumber(maxPlayers));
			NewSession->GetNumPlayersTextBox()->SetText(playersText);
			
			NewSession->OnSessionSelectedDelegate.AddDynamic(this, &ThisClass::OnSessionEntrySelected);
			AddSession(NewSession);



			MultiplayerSessionsSubsystem->JoinSession(Result);
			//return;
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

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Red,
			FString(TEXT("Successfully added child to scroll box."))
		);
	}
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



void ULobbyMenu::LobbyMenuSetup()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);

	//UWorld* World = GetWorld();
	//if (World)
	//{
	//	APlayerController* PlayerController = World->GetFirstPlayerController();
	//	if (PlayerController)
	//	{
	//		FInputModeUIOnly InputModeData;
	//		InputModeData.SetWidgetToFocus(TakeWidget());
	//		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//		PlayerController->SetInputMode(InputModeData);
	//		PlayerController->SetShowMouseCursor(true);
	//	}
	//}


}

void ULobbyMenu::JoinButtonClicked()
{

	/* Logic for each session we find after searching */
	USessionEntry* NewSession = CreateWidget<USessionEntry>(this, SessionEntryClass);
	NewSession->SessionEntrySetup();
	NewSession->GetSessionEntryButton()->SetStyle(NormalButtonStyle);
	NewSession->OnSessionSelectedDelegate.AddDynamic(this, &ThisClass::OnSessionEntrySelected);
	AddSession(NewSession);
	
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
