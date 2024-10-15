// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionEntry.h"
#include "OnlineSessionSettings.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void USessionEntry::SessionEntrySetup()
{
	if (SessionSearchResult.IsValid() && Text_HostName && Text_LobbyName && Text_MapName && Text_NumPlayers && Text_Ping)
	{
		FString GameMode;
		FString LobbyName;
		FString MapName;
		SessionSearchResult.Session.SessionSettings.Get(FName("GameMode"), GameMode); // Gets the GameMode of the sessions and outputs it to GameMode
		SessionSearchResult.Session.SessionSettings.Get(FName("LobbyName"), LobbyName); // Gets the LobbyName of the sessions and outputs it to LobbyName
		SessionSearchResult.Session.SessionSettings.Get(FName("MapName"), MapName); // Gets the MapName of the sessions and outputs it to MapName
		int32 maxPlayers = SessionSearchResult.Session.SessionSettings.NumPublicConnections; // Calculate max allowed players by querying the number of public connections
		int32 numPlayers = maxPlayers - SessionSearchResult.Session.NumOpenPublicConnections; // Calculate current players in lobby by subtracting the number of open public connections from the max allowed public connections
		FText playersText = FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(numPlayers), FText::AsNumber(maxPlayers));

		Text_HostName->SetText(FText::FromString(SessionSearchResult.Session.OwningUserName));
		Text_LobbyName->SetText(LobbyName.IsEmpty() ? FText::FromString(TEXT("UNDEFINED")) : FText::FromString(LobbyName));
		Text_MapName->SetText(FText::FromString(MapName));
		Text_GameMode->SetText(FText::FromString(GameMode));
		Text_Ping->SetText(FText::AsNumber(SessionSearchResult.PingInMs));
		Text_NumPlayers->SetText(playersText);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Red,
			FString(TEXT("Setting up entry listing, but SessionSearchResult was not valid."))
		);
	}

	// Bind button click callback
	Button_SessionEntry->OnPressed.AddDynamic(this, &ThisClass::OnSessionSelected);
}

void USessionEntry::OnSessionSelected()
{
	// Broadcast a reference of itself to the LobbyMenu
	OnSessionSelectedDelegate.Broadcast(this);
}

UButton* USessionEntry::GetSessionEntryButton()
{
	return Button_SessionEntry;
}

UTextBlock* USessionEntry::GetMapNameTextBox()
{
	return Text_MapName;
}

UTextBlock* USessionEntry::GetLobbyTextBox()
{
	return Text_LobbyName;
}

UTextBlock* USessionEntry::GetNumPlayersTextBox()
{
	return Text_NumPlayers;
}

UTextBlock* USessionEntry::GetPingTextBox()
{
	return Text_Ping;
}

