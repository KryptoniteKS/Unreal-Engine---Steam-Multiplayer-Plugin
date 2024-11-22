// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerPauseMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResumeButtonClicked);

UCLASS(Blueprintable)
class MULTIPLAYERSESSIONS_API UPlayerPauseMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnResumeButtonClicked OnResumeButtonClickedDelegate;

protected:
	virtual bool Initialize();

private:
	UFUNCTION()
	void BindButtonCallbacks();
	UFUNCTION()
	void OnResumeButtonClicked();
	UFUNCTION()
	void OnOptionsButtonClicked();
	UFUNCTION()
	void OnInviteButtonClicked();
	UFUNCTION()
	void OnMainMenuButtonClicked();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSoftObjectPtr<UWorld> MainMenuMap;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UButton* Btn_Resume;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UButton* Btn_Options;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UButton* Btn_Invite;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UButton* Btn_MainMenu;

	UPROPERTY()
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

};
