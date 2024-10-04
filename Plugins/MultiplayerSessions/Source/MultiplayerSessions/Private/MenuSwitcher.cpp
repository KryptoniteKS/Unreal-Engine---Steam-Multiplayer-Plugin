// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSwitcher.h"
#include "Menu.h"
#include "Components/WidgetSwitcher.h"
#include "LobbyMenu.h"

bool UMenuSwitcher::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (Menu)
	{
		Menu->OnJoinButtonClickedDelegate.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Red,
			FString(TEXT("Menu was not valid!"))
		);
	}

	return true;

	
}

void UMenuSwitcher::JoinButtonClicked()
{
	WidgetSwitcher->SetActiveWidget(Cast<UWidget>(LobbyMenu));
}
