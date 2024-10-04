// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuSwitcher.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API UMenuSwitcher : public UUserWidget
{
	GENERATED_BODY()

public:

protected:
	virtual bool Initialize() override;

private:
	/* Widget Controls */
	UPROPERTY(meta = (BindWidget))
	class UMenu* Menu;
	UPROPERTY(meta = (BindWidget))
	class ULobbyMenu* LobbyMenu;
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;


	/* Callback functions for button delegates */
	UFUNCTION()
	void JoinButtonClicked();
	UFUNCTION()
	void LobbyBackButtonClicked();
	
};
