// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include <steam/isteamfriends.h>

#include "Menu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJoinButtonClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHostButtonClicked);

UCLASS()
class MULTIPLAYERSESSIONS_API UMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
#pragma region Button Click Delegates
	FOnJoinButtonClicked OnJoinButtonClickedDelegate;
	FOnHostButtonClicked OnHostButtonClickedDelegate;
#pragma endregion

protected:
#pragma region Initialization
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;
#pragma endregion

private:
#pragma region Buttons
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;
#pragma endregion

#pragma region Button Callbacks
	UFUNCTION()
	void HostButtonClicked();
	UFUNCTION()
	void JoinButtonClicked();
#pragma endregion

	void MenuTearDown();
};
