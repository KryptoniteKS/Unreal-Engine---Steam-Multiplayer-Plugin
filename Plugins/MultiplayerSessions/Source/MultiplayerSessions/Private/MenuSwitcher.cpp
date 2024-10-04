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
		// Bind our callback function to the delegate for the Menu's Join button
		Menu->OnJoinButtonClickedDelegate.AddDynamic(this, &ThisClass::JoinButtonClicked);
	}

	if (LobbyMenu)
	{
		// Bind our callback function to the delegate for the Lobby's Back button
		LobbyMenu->OnBackButtonClickedDelegate.AddDynamic(this, &ThisClass::LobbyBackButtonClicked);
	}

	return true;
}

void UMenuSwitcher::JoinButtonClicked()
{
	WidgetSwitcher->SetActiveWidget(Cast<UWidget>(LobbyMenu));
}

void UMenuSwitcher::LobbyBackButtonClicked()
{
	WidgetSwitcher->SetActiveWidget(Cast<UWidget>(Menu));
}
