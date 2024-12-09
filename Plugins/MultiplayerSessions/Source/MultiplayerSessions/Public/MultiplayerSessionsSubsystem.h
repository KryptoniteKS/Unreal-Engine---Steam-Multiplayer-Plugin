// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SteamShared.h"
#include "Tickable.h"
THIRD_PARTY_INCLUDES_START
#include <steam/steam_api.h>
#include <steam/isteammatchmaking.h>
#include <steam/isteamuser.h>
#include <steam/isteamfriends.h>
THIRD_PARTY_INCLUDES_END

#include "MultiplayerSessionsSubsystem.generated.h"


/* Declaring our own custom delegates for the Menu class to bind callbacks to */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionsComplete, const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWassuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, EOnJoinSessionCompleteResult::Type Result);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestLobbyListComplete, const TArray<FLobbyEntry>&, LobbyData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateLobbyComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnJoinLobbyComplete, bool, bWasSuccessful);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameLobbyJoinRequested, FSteamId, SteamId, FSteamId, LobbyId);



UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
#pragma region Initialization
	UMultiplayerSessionsSubsystem();
	~UMultiplayerSessionsSubsystem();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
#pragma endregion

	UFUNCTION()
	void ParseInviteId(); // Takes Steam ID from command line and joins that lobby and listen server

#pragma region Getters
	UFUNCTION()
	FString GetMapNameKey() { return Key_MapName; }
	UFUNCTION()
	FString GetLobbyNameKey() { return Key_LobbyName; }
	UFUNCTION()
	FString GetGameModeKey() { return Key_GameMode; }
	UFUNCTION()
	FSteamId GetCurrentLobbyId() { return CurrentLobbyId; }
#pragma endregion

#pragma region Steam Functions
	UFUNCTION()
	void RequestLobbyList();
	UFUNCTION()
	void CreateLobby(FString MapName, FString LobbyName, FString GameMode, int32 MaxNumPlayers);
	UFUNCTION()
	void JoinLobby(FSteamId LobbyID);
	UFUNCTION()
	void JoinListenServer(FSteamId SessionID); // On a listen server, the SessionID is the SteamID of the host player
	UFUNCTION()
	void JoinCurrentLobbyListenServer(); // Joins the listen server connected to the last lobby joined.
	UFUNCTION()
	void ActivateInviteOverlay();
	UFUNCTION()
	void LeaveCurrentLobby();
#pragma endregion

#pragma region Steam Delegates
	UPROPERTY(BlueprintAssignable, Category = "Steam Callbacks")
	FOnRequestLobbyListComplete OnRequestLobbyListComplete;
	UPROPERTY(BlueprintAssignable, Category = "Steam Callbacks")
	FOnCreateLobbyComplete OnCreateLobbyComplete;
	UPROPERTY(BlueprintAssignable, Category = "Steam Callbacks")
	FOnJoinLobbyComplete OnJoinLobbyComplete; // TODO: Bind from LobbyMenu
#pragma endregion

#pragma region Steam Delegate Callbacks
	UFUNCTION()
	void OnRequestLobbyList(int32 LobbiesMatching);
	UFUNCTION()
	void OnCreateLobby(TEnumAsByte<ESteamResult> Result, FSteamId LobbyID);
	UFUNCTION()
	void OnJoinLobby(FSteamId LobbyId, bool bLocked, TEnumAsByte<ESteamChatRoomEnterResponse> ChatRoomEnterResponse);
#pragma endregion

private:
#pragma region Steam API Callback Registrations
	/* All Steam API callbacks are bound in the initialization logic for this class. Remember manual callbacks must be unregistered within the de-initializer for this class. */

	/* This callback is triggered when a player accepts a steam invite while the game is already running. */
	STEAM_CALLBACK_MANUAL(UMultiplayerSessionsSubsystem, OnGameLobbyJoinRequestedCallback, GameLobbyJoinRequested_t, m_CallbackGameLobbyJoinRequested);
#pragma endregion
		
#pragma region Last Settings
	int32 LastMaxNumPlayers;
	FString LastGameMode;
	FString LastMapName;
	FString LastLobbyName;

	UPROPERTY()
	FSteamId LastLobbyJoined; // Steam ID for the last lobby we joined (generally also the current lobby).
#pragma endregion

#pragma region Lobby Metadata
		UPROPERTY()
		FLobbyMetadata LobbyMetadata; // Lobby metadata to be set on the lobby currently being created

		FString Key_MapName = TEXT("MapName");
		FString Key_LobbyName = TEXT("LobbyName");
		FString Key_GameMode = TEXT("GameMode");
		FString Key_HostName = TEXT("HostName");
#pragma endregion

#pragma region Steam Async Objects
		UPROPERTY()
		class USteamRequestLobbyListAsync* SteamRequestLobbyListAsync;
		UPROPERTY()
		class USteamCreateLobbyAsync* SteamCreateLobbyAsync;
		UPROPERTY()
		class USteamJoinLobbyAsync* SteamJoinLobbyAsync;
#pragma endregion

	UPROPERTY()
	FSteamId CurrentLobbyId; // For my particular game, I only want to support having one active lobby at a time. This can be a list if you want multiple

	UFUNCTION()
	void InitializeSteamCallbacks();

};
