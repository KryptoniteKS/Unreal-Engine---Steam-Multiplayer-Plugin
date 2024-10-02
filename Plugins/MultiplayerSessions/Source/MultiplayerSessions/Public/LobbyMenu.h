// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "LobbyMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API ULobbyMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void LobbyMenuSetup();
	virtual bool Initialize();

	UFUNCTION(BlueprintCallable, Category = "Session Management")
	void AddSession(USessionEntry* Session);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Session Management")
	class USessionEntry* SelectedSession;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Session Management")
	TArray<USessionEntry*> Sessions;

	UFUNCTION()
	void OnSessionEntrySelected(USessionEntry* Session);

protected:
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* SearchButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UScrollBox* ScrollBox_Sessions;
	UPROPERTY(EditAnywhere)
	TSubclassOf<USessionEntry> SessionEntryClass;

	UFUNCTION()
	void JoinButtonClicked();
	UFUNCTION()
	void SearchButtonClicked();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FButtonStyle SelectedButtonStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FButtonStyle NormalButtonStyle;

	// Our subsystem designed to handle all online session functionality
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;


};
