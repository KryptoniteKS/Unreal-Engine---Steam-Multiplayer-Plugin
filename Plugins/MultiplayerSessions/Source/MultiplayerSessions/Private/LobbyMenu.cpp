// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMenu.h"
#include "Components/Button.h"
#include "SessionEntry.h"
#include "Components/ScrollBox.h"

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

	return true;
}

void ULobbyMenu::AddSession(USessionEntry* Session)
{
	// Must perform a cast for this method to work properly even though USessionEntry is a UUserWidget
	ScrollBox_Sessions->AddChild(Cast<UWidget>(Session));

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
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Red,
			FString(TEXT("Join Button Clicked"))
		);
	}

	USessionEntry* NewSession = CreateWidget<USessionEntry>(this, SessionEntryClass);
	AddSession(NewSession);
}
