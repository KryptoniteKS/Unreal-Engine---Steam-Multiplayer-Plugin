// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HostMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHostMenuBackButtonClicked);

UCLASS()
class MULTIPLAYERSESSIONS_API UHostMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/* Initialization Logic */
	virtual bool Initialize();

	/* Custom Delegates for our Buttons */
	FOnHostMenuBackButtonClicked OnBackButtonClickedDelegate;

private:
	/* Widget Controls */
	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* Combo_Maps;

	/* Button Callbacks */
	UFUNCTION()
	void BackButtonClicked();
};
