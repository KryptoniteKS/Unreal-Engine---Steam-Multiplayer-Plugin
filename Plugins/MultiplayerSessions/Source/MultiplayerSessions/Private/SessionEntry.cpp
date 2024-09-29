// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionEntry.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void USessionEntry::SessionEntrySetup()
{
	if (Text_LobbyName && Text_MapName && Text_NumPlayers && Text_Ping) 
	{
		Text_LobbyName->SetText(FText::FromString("Test Lobby"));
		Text_MapName->SetText(FText::FromString("Test Map Name"));
		Text_NumPlayers->SetText(FText::FromString("12/64"));
		Text_Ping->SetText(FText::FromString("43"));
	}

	// Bind button click callback
	Button_SessionEntry->OnPressed.AddDynamic(this, &ThisClass::OnSessionSelected);

}

UButton* USessionEntry::GetSessionEntryButton()
{
	return Button_SessionEntry;
}

void USessionEntry::OnSessionSelected()
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		15.f,
		FColor::Blue,
		FString(TEXT("Broadcasting..."))
	);

	// Broadcast a reference to itself to the LobbyMenu
	OnSessionSelectedDelegate.Broadcast(this);
}

void USessionEntry::NativeConstruct()
{
	Super::NativeConstruct();
}
