// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionMenu.h"
#include "SessionEntryDataWrapper.h"

void USessionMenu::AddItemToList(UObject* SessionItem)
{
	USessionEntryDataWrapper* Item = Cast<USessionEntryDataWrapper>(SessionItem);
	if (Item)
	{
		List->AddItem(Item);
	}
}

void USessionMenu::MenuSetup(UMenu* InMenu)
{
	Menu = InMenu;

	FSessionInfo TestSession;
	TestSession.MapName = FString(TEXT("Herobrine's Lair"));
	TestSession.LobbyName = FString(TEXT("Herobrine's 1v1 Mega Lobby"));
	TestSession.NumPlayers = FString(TEXT("4/12"));
	TestSession.HostName = FString(TEXT("Herobrine"));
	TestSession.Ping = 9999;

	AddToViewport();

	USessionEntryDataWrapper* Wrapper = NewObject<USessionEntryDataWrapper>();
	if (Wrapper != nullptr && List != nullptr)
	{
		Wrapper->SessionInfo = TestSession;
		List->AddItem(Wrapper);
		AddToViewport();
	}

}
