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
	/* Custom Delegates for our Buttons */
	FOnHostMenuBackButtonClicked OnBackButtonClickedDelegate;

protected:
	/* Initialization Logic */
	virtual bool Initialize();
	void FillMapsDropdown();

	/* Custom Callbacks for the Multiplayer Subsystem */
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);

private:
	/* Widget Controls */
	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UEditableTextBox* Text_LobbyName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UEditableTextBox* Text_NumPlayers;
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* Combo_Maps;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UComboBoxString* Combo_GameMode;

	/* Button Callbacks */
	UFUNCTION()
	void BackButtonClicked();
	UFUNCTION()
	void HostButtonClicked();

	/* Lobby Values */
	FString MapName;
	FString LobbyName;
	FString GameMode;
	int32 MaxNumPlayers;
	TArray<FString> ServerMaps;

	/* Default Values */
	UPROPERTY(EditAnywhere, Category = "Server Defaults")
	FString DefaultLobbyName = TEXT("My Default Lobby");
	UPROPERTY(EditAnywhere, Category = "Server Defaults")
	int32 DefaultMaxNumPlayers = 4;
	UPROPERTY(EditAnywhere, Category = "Server Defaults")
	FString ServerMapsDirectory = TEXT("Maps/Server"); // Directory beginning from Game/Content/ directory

	/* Networking Variables */
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;

	/* Helper Functions */
	FString FormatMapName(const FString& OriginalName, bool bReturnLiteralName);
	void EnableControls(bool bShouldEnable);

};
