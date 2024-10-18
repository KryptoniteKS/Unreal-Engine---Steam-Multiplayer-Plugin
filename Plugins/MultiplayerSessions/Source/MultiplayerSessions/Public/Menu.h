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
	/* Initialization Logic */
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int32 NumberOfPublicConnections = 4, FString TypeOfMatch = FString(TEXT("FreeForAll")), FString LobbyPath = FString(TEXT("/Game/ThirdPerson/Maps/Lobby")));

	/* Public Getters/Setters */
	UFUNCTION(BlueprintCallable)
	class UButton* GetJoinButton();

	/* Delegates */
	FOnJoinButtonClicked OnJoinButtonClickedDelegate;
	FOnHostButtonClicked OnHostButtonClickedDelegate;

protected:
	/* Overrides */
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

	/* Callbacks for the custom delegates on the MultiplayerSessionsSubsystem */
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
	void OnStartSession(bool bWasSuccessful);

private:
	/* Widget Controls */
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;

	/* Button Callbacks */
	UFUNCTION()
	void HostButtonClicked();
	UFUNCTION()
	void JoinButtonClicked();

	/* Destroy Logic */
	void MenuTearDown();

	/* Networking Variables */
	class UMultiplayerSessionsSubsystem* MultiplayerSessionsSubsystem;
	int32 NumPublicConnections{4};
	FString MatchType{ TEXT("FreeForAll") };
	FString PathToLobby{ TEXT("") };
};
