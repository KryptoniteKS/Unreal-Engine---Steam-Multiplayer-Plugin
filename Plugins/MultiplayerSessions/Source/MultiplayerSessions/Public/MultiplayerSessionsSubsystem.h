// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SteamShared.h"
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



UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UMultiplayerSessionsSubsystem();

	/* To handle Session functionality. The Menu class will call these */
	void CreateSession(FString MapName, FString LobbyName, FString GameMode, int32 MaxNumPlayers);
	void FindSessions(int32 MaxSearchResults);
	void JoinSession(const FOnlineSessionSearchResult& SessionResult);
	void DestroySession();
	void StartSession();

	UFUNCTION()
	FString GetMapNameKey() { return Key_MapName; }
	UFUNCTION()
	FString GetLobbyNameKey() { return Key_LobbyName; }
	UFUNCTION()
	FString GetGameModeKey() { return Key_GameMode; }

	UFUNCTION()
	void RequestLobbyList();
	UFUNCTION()
	void OnRequestLobbyList(int32 LobbiesMatching);
	UFUNCTION()
	void CreateLobby(FString MapName, FString LobbyName, FString GameMode, int32 MaxNumPlayers);
	UFUNCTION()
	void OnCreateLobby(TEnumAsByte<ESteamResult> Result, FSteamId LobbyID);
	UFUNCTION()
	void JoinLobby(FSteamId LobbyID);
	UFUNCTION()
	void JoinListenServer(FSteamId SessionID); // On a listen server, the SessionID is the SteamID of the host player
	UFUNCTION()
	void JoinCurrentLobbyListenServer(); // Joins the listen server connected to the last lobby joined.
	UFUNCTION()
	void OnJoinLobby(FSteamId LobbyId, bool bLocked, TEnumAsByte<ESteamChatRoomEnterResponse> ChatRoomEnterResponse);

		/* Our own custom delegates for the Menu classes to bind callbacks to */
	FMultiplayerOnCreateSessionComplete MultiplayerOnCreateSessionComplete;
	FMultiplayerOnFindSessionsComplete MultiplayerOnFindSessionsComplete;
	FMultiplayerOnJoinSessionComplete MultiplayerOnJoinSessionComplete;
	FMultiplayerOnDestroySessionComplete MultiplayerOnDestroySessionComplete;
	FMultiplayerOnStartSessionComplete MultiplayerOnStartSessionComplete;
	FOnRequestLobbyListComplete OnRequestLobbyListComplete;
	FOnCreateLobbyComplete OnCreateLobbyComplete;
	FOnJoinLobbyComplete OnJoinLobbyComplete; // TODO: Bind from LobbyMenu

protected:
	/* Internal callbacks for the delegates we will add to the Online Session Interface delegate list. These don't need to be called outside this class. */
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);

private:
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	/* To add to the online session interface delegate list. We will bind our MultiplayerSessionsSubsystem internal callbacks to these.
	*  The FDelegateHandle declarations are to store the handles to the delegates after we add delegates to the delegate list. This will allow us to remove
	*  the delegates from the delegate list when we are done using them. */
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;

	FOnFindSessionsCompleteDelegate FindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;

	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;

	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;

	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;

	bool bCreateSessionOnDestroy{ false };
	int32 LastMaxNumPlayers;
	FString LastGameMode;
	FString LastMapName;
	FString LastLobbyName;

	FSteamId LastLobbyJoined; // Keep track of the Steam ID for the last lobby we joined (generally also the current lobby).

	FString Key_MapName = TEXT("MapName");
	FString Key_LobbyName = TEXT("LobbyName");
	FString Key_GameMode = TEXT("GameMode");
	FString Key_HostName = TEXT("HostName");

	UPROPERTY()
	class USteamRequestLobbyListAsync* SteamRequestLobbyListAsync;
	UPROPERTY()
	class USteamCreateLobbyAsync* SteamCreateLobbyAsync;
	UPROPERTY()
	class USteamJoinLobbyAsync* SteamJoinLobbyAsync;

	UPROPERTY()
	FSteamId CurrentLobbyId; // Only supports one single lobby right now. Steam supports one player owning multiple lobbies, so may need to implement later
	UPROPERTY()
	FLobbyMetadata LobbyMetadata; // Lobby metadata to be set on the lobby currently being created

};
