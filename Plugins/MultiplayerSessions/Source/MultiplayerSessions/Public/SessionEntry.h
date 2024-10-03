// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "SessionEntry.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSessionSelected, class USessionEntry*, Session);

UCLASS()
class MULTIPLAYERSESSIONS_API USessionEntry : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct();

	UFUNCTION(BlueprintCallable)
	void SessionEntrySetup();
	class UButton* GetSessionEntryButton();
	FOnSessionSelected OnSessionSelectedDelegate;
	UFUNCTION(BlueprintCallable)
	void OnSessionSelected();

	/* Getters and Setters for Widget Controls */
	UFUNCTION(BlueprintCallable)
	class UTextBlock* GetMapNameTextBox();
	UFUNCTION(BlueprintCallable)
	class UTextBlock* GetLobbyTextBox();
	UFUNCTION(BlueprintCallable)
	class UTextBlock* GetNumPlayersTextBox();
	UFUNCTION(BlueprintCallable)
	class UTextBlock* GetPingTextBox();

	/* SessionSearchResult Getter and Setter */
	void SetSessionSearchResult(const FOnlineSessionSearchResult& SearchResult) { SessionSearchResult = SearchResult; }
	const FOnlineSessionSearchResult& GetSessionSearchResult() const { return SessionSearchResult; }

private:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UTextBlock* Text_LobbyName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UTextBlock* Text_MapName;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UTextBlock* Text_NumPlayers;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UTextBlock* Text_Ping;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UButton* Button_SessionEntry;

	FOnlineSessionSearchResult SessionSearchResult;
};
