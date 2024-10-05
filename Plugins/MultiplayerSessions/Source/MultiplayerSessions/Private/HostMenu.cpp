// Fill out your copyright notice in the Description page of Project Settings.


#include "HostMenu.h"
#include "Components/Button.h"

bool UHostMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &ThisClass::BackButtonClicked);
	}

	return true;
}

void UHostMenu::BackButtonClicked()
{
	OnBackButtonClickedDelegate.Broadcast();
}
