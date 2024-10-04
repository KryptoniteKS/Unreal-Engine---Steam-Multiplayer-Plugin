// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionEntry.h"
#include "OnlineSessionSettings.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void USessionEntry::SessionEntrySetup()
{
	if (SessionSearchResult.IsValid() && Text_LobbyName && Text_MapName && Text_NumPlayers && Text_Ping)
	{
		Text_Ping->SetText(FText::AsNumber(SessionSearchResult.PingInMs));
		Text_LobbyName->SetText(FText::FromString(SessionSearchResult.Session.OwningUserName));
		int32 maxPlayers = SessionSearchResult.Session.SessionSettings.NumPublicConnections;
		int32 numPlayers = maxPlayers - SessionSearchResult.Session.NumOpenPublicConnections;
		FText playersText = FText::Format(FText::FromString(TEXT("{0}/{1}")), FText::AsNumber(numPlayers), FText::AsNumber(maxPlayers));
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

