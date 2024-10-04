// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "LobbyMenu.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBackButtonClicked);

UCLASS()
class MULTIPLAYERSESSIONS_API ULobbyMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/* Initialization Logic */
	virtual bool Initialize();

	/* Session Functions */
	UFUNCTION(BlueprintCallable, Category = "Session Management")
	void AddSession(USessionEntry* Session);
	void ClearSessions();

	/* Session Variables */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Session Management")
	class USessionEntry* SelectedSession;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Session Management")
	TArray<USessionEntry*> Sessions;

	/* Custom Callbacks for Delegates */
	UFUNCTION()
	void OnSessionEntrySelected(USessionEntry* Session);

	/* Delegates */
	FOnBackButtonClicked OnBackButtonClickedDelegate;

protected:
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);

private:
	/* Widget Controls */
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* SearchButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* BackButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UScrollBox* ScrollBox_Sessions;
	UPROPERTY(EditAnywhere)
	TSubclassOf<USessionEntry> SessionEntryClass;


	/* Button Callbacks */
	UFUNCTION()
	void JoinButtonClicked();
	UFUNCTION()
	void SearchButtonClicked();
	UFUNCTION()
	void BackButtonClicked();


	/* Button Styles for Selection states */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FButtonStyle SelectedButtonStyle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FButtonStyle NormalButtonStyle;


	/* Networking Variables */
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
};
