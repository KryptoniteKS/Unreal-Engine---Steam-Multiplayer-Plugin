// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSwitcher.h"
#include "Menu.h"
#include "Components/WidgetSwitcher.h"
#include "LobbyMenu.h"
#include "HostMenu.h"

bool UMenuSwitcher::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (Menu)
	{
		// Bind our MenuSwitcher callback functions to the delegates on the Menu buttons
		Menu->OnJoinButtonClickedDelegate.AddDynamic(this, &ThisClass::JoinButtonClicked);
		Menu->OnHostButtonClickedDelegate.AddDynamic(this, &ThisClass::HostButtonClicked);
	}

	if (LobbyMenu)
	{
		// Bind our callback function to the delegate for the Lobby Menu's Back button
		LobbyMenu->OnBackButtonClickedDelegate.AddDynamic(this, &ThisClass::LobbyMenuBackButtonClicked);
	}

	if (HostMenu)
	{
		// Bind our callback function to the delegate for the HostMenu's Back button
		HostMenu->OnBackButtonClickedDelegate.AddDynamic(this, &ThisClass::HostMenuBackButtonClicked);
	}

	return true;
}

void UMenuSwitcher::JoinButtonClicked()
{
	WidgetSwitcher->SetActiveWidget(Cast<UWidget>(LobbyMenu));
}

void UMenuSwitcher::HostButtonClicked()
{
	WidgetSwitcher->SetActiveWidget(Cast<UWidget>(HostMenu));
}

void UMenuSwitcher::LobbyMenuBackButtonClicked()
{
	WidgetSwitcher->SetActiveWidget(Cast<UWidget>(Menu));
}

void UMenuSwitcher::HostMenuBackButtonClicked()
{
	WidgetSwitcher->SetActiveWidget(Cast<UWidget>(Menu));
}
