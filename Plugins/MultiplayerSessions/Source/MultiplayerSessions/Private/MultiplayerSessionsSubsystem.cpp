// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "SteamRequestLobbyListAsync.h"
#include "SteamCreateLobbyAsync.h"
#include "SteamJoinLobbyAsync.h"
#include "SteamMatchmaking.h"
#include "Kismet/GameplayStatics.h"
#include "SteamFriends.h"

#pragma region Initialization
UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem()
{
	SteamAPI_Init();

	InitializeSteamCallbacks();
}

void UMultiplayerSessionsSubsystem::InitializeSteamCallbacks()
{
	SteamRequestLobbyListAsync = NewObject<USteamRequestLobbyListAsync>();
	if (SteamRequestLobbyListAsync != nullptr)
	{
		SteamRequestLobbyListAsync->OnSuccess.AddDynamic(this, &ThisClass::OnRequestLobbyList);
		SteamRequestLobbyListAsync->OnFailure.AddDynamic(this, &ThisClass::OnRequestLobbyList);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SteamRequestLobbyListAsync was null!"));
	}

	SteamCreateLobbyAsync = NewObject<USteamCreateLobbyAsync>();
	if (SteamCreateLobbyAsync != nullptr)
	{
		SteamCreateLobbyAsync->OnSuccess.AddDynamic(this, &ThisClass::OnCreateLobby);
		SteamCreateLobbyAsync->OnFailure.AddDynamic(this, &ThisClass::OnCreateLobby);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SteamCreateLobbyAsync was null!"));
	}

	SteamJoinLobbyAsync = NewObject<USteamJoinLobbyAsync>();
	if (SteamJoinLobbyAsync != nullptr)
	{
		SteamJoinLobbyAsync->OnSuccess.AddDynamic(this, &ThisClass::OnJoinLobby);
		SteamJoinLobbyAsync->OnFailure.AddDynamic(this, &ThisClass::OnJoinLobby);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SteamJoinLobbyAsync was null!"));
	}

	if (SteamFriends())
	{
		m_CallbackGameLobbyJoinRequested.Register(this, &ThisClass::OnGameLobbyJoinRequestedCallback);
	}
}

UMultiplayerSessionsSubsystem::~UMultiplayerSessionsSubsystem()
{
	m_CallbackGameLobbyJoinRequested.Unregister();
}

void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// We need to get the Steam ID for the lobby from any existing invite if one exists and join that lobby and server
	ParseInviteId();
}

void UMultiplayerSessionsSubsystem::ParseInviteId()
{
	FString CommandLine = FCommandLine::Get();
	FString LobbyCommand = TEXT("+connect_lobby");
	FString LobbyId;

	// Check if command line contains our LobbyCommand. If so, output to LobbyId
	if (FParse::Value(*CommandLine, *LobbyCommand, LobbyId))
	{
		LobbyId = LobbyId.TrimStartAndEnd();
		int64 SteamLobbyId = FCString::Strtoi64(*LobbyId, nullptr, 10);

		if (SteamLobbyId != 0)
		{
			UE_LOG(LogTemp, Log, TEXT("Successfully retrieved Steam Lobby ID from invite: %lld"), SteamLobbyId);
			JoinLobby(FSteamId(SteamLobbyId));
			JoinCurrentLobbyListenServer();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("An invite was found, but the Steam ID returned was 0. Moving forward with default startup..."));
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("No invite found in command line. Moving forward with default startup..."));
	}
}
#pragma endregion

#pragma region Lobby Functions
void UMultiplayerSessionsSubsystem::CreateLobby(FString MapName, FString LobbyName, FString GameMode, int32 MaxNumPlayers)
{
	if (SteamMatchmaking() == nullptr || SteamFriends() == nullptr)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Setting local lobby metadata on the Subsystem..."));

	LobbyMetadata.Reset();

	LobbyMetadata.GameMode = GameMode;
	LobbyMetadata.MapName = MapName; // Formatted map name
	LobbyMetadata.LobbyName = LobbyName;
	LobbyMetadata.MaxNumPlayers = MaxNumPlayers;
	LobbyMetadata.HostName = ANSI_TO_TCHAR(SteamFriends()->GetPersonaName()); // TODO: Create my own interface

	SteamCreateLobbyAsync->CreateLobby(MaxNumPlayers, ESteamLobbyType::LobbyTypePublic);
}

void UMultiplayerSessionsSubsystem::RequestLobbyList()
{
	SteamRequestLobbyListAsync->RequestLobbyList();
}

void UMultiplayerSessionsSubsystem::JoinLobby(FSteamId LobbyID)
{
	// TODO: Before we join ANY lobbies, let's leave any existing lobby we may be in. If we stay in any orphaned lobbies, they will not disappear from the search list.

	SteamJoinLobbyAsync->JoinLobby(LobbyID);
	// Should eventually trigger our OnJoinLobby callback with the Steam API response
}

void UMultiplayerSessionsSubsystem::LeaveCurrentLobby()
{
	USteamMatchmaking::LeaveLobby(CurrentLobbyId);
}
#pragma endregion

#pragma region Lobby Function Callbacks
void UMultiplayerSessionsSubsystem::OnCreateLobby(TEnumAsByte<ESteamResult> Result, FSteamId LobbyID)
{
	UE_LOG(LogTemp, Warning, TEXT("CreateLobby broadcast received by our Subsystem."));

	// Look at result. Set Lobby Metadata if good. Otherwise, broadcast failure
	if (Result == ESteamResult::ResultOK)
	{
		UE_LOG(LogTemp, Warning, TEXT("Result from CreateLobby was good. Setting Lobby Metadata now..."));

		// Set metadata
		UE_LOG(LogTemp, Warning, TEXT("Setting Lobby Metadata - Key: %s - Value: %s"), *Key_GameMode, *LobbyMetadata.GameMode);
		USteamMatchmaking::SetLobbyData(LobbyID, Key_GameMode, LobbyMetadata.GameMode);
		UE_LOG(LogTemp, Warning, TEXT("Setting Lobby Metadata - Key: %s - Value: %s"), *Key_LobbyName, *LobbyMetadata.LobbyName);
		USteamMatchmaking::SetLobbyData(LobbyID, Key_LobbyName, LobbyMetadata.LobbyName);
		UE_LOG(LogTemp, Warning, TEXT("Setting Lobby Metadata - Key: %s - Value: %s"), *Key_MapName, *LobbyMetadata.MapName);
		USteamMatchmaking::SetLobbyData(LobbyID, Key_MapName, LobbyMetadata.MapName); // Literal map name, unformatted
		UE_LOG(LogTemp, Warning, TEXT("Setting Lobby Metadata - Key: %s - Value: %s"), *Key_HostName, *LobbyMetadata.HostName);
		USteamMatchmaking::SetLobbyData(LobbyID, Key_HostName, LobbyMetadata.HostName);

		OnCreateLobbyComplete.Broadcast(true);
	}
	else
	{
		OnCreateLobbyComplete.Broadcast(false);
	}
}

void UMultiplayerSessionsSubsystem::OnRequestLobbyList(int32 LobbiesMatching)
{
	if (LobbiesMatching <= 0)
	{
		return;
	}

	UE_LOG(LogTemp, Error, TEXT("At least one lobby was found. Looping through them..."));

	// Loop through the sessions by using GetLobbyByIndex
	TArray<FLobbyEntry> LobbyData;
	for (int curLobby = 0; curLobby < LobbiesMatching; curLobby++)
	{
		auto LobbyId = USteamMatchmaking::GetLobbyByIndex(curLobby);

		FLobbyEntry LobbyEntry;
		LobbyEntry.LobbyID = LobbyId;
		LobbyEntry.MaxPlayers = USteamMatchmaking::GetLobbyMemberLimit(LobbyId);
		LobbyEntry.NumPlayers = USteamMatchmaking::GetNumLobbyMembers(LobbyId);
		LobbyEntry.Ping = 72;
		LobbyEntry.HostName = USteamMatchmaking::GetLobbyData(LobbyId, Key_HostName);
		LobbyEntry.GameMode = USteamMatchmaking::GetLobbyData(LobbyId, Key_GameMode);
		LobbyEntry.LobbyName = USteamMatchmaking::GetLobbyData(LobbyId, Key_LobbyName);
		LobbyEntry.MapName = USteamMatchmaking::GetLobbyData(LobbyId, Key_MapName);

		// After we have created our structs, add them to a list to be broadcast at the end of the loop. LobbyMenu will subscribe to this
		UE_LOG(LogTemp, Error, TEXT("Adding Lobby with index %d and Lobby ID %lu to our menu."), curLobby, LobbyId.Result);
		LobbyData.Add(LobbyEntry);
	}


	if (LobbyData.Num() > 0)
	{
		UE_LOG(LogTemp, Error, TEXT("There should be %d lobbies visible on our menu..."), LobbyData.Num());
	}

	UE_LOG(LogTemp, Error, TEXT("Broadcasting the list of lobbies to our Lobby Menu."));
	OnRequestLobbyListComplete.Broadcast(LobbyData);
}

void UMultiplayerSessionsSubsystem::OnJoinLobby(FSteamId LobbyId, bool bLocked, TEnumAsByte<ESteamChatRoomEnterResponse> ChatRoomEnterResponse)
{
	// If we have successfully entered the lobby, or "Chat room", broadcast that back to the LobbyMenu
	if (ChatRoomEnterResponse == ESteamChatRoomEnterResponse::ChatRoomEnterResponseSuccess)
	{
		LastLobbyJoined = LobbyId;
		OnJoinLobbyComplete.Broadcast(true);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Steam API SteamChatRoomEnterResponse does not indicate success."));
		OnJoinLobbyComplete.Broadcast(false);
	}
}
#pragma endregion

#pragma region Steam API Registered Callbacks
void UMultiplayerSessionsSubsystem::OnGameLobbyJoinRequestedCallback(GameLobbyJoinRequested_t* pCallback)
{
	UE_LOG(LogTemp, Warning, TEXT("Received GameLobbyJoinRequested callabck from Steam. Attempting to connect to requested lobby and listen server..."));
	JoinLobby(FSteamId(pCallback->m_steamIDLobby));
	JoinCurrentLobbyListenServer();
}
#pragma endregion

#pragma region Steam Functions
void UMultiplayerSessionsSubsystem::ActivateInviteOverlay()
{
	USteamFriends::ActivateGameOverlayInviteDialog(CurrentLobbyId.Result);
}
#pragma endregion

#pragma region Listen Server Functions
void UMultiplayerSessionsSubsystem::JoinListenServer(FSteamId SessionID)
{
	if (SessionID.Result == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Steam Session ID!"));
		return;
	}

	FString LevelName = FString::Printf(TEXT("steam.%llu"), SessionID.Result);
	UWorld* World = GetWorld();
	if (World)
	{
		UGameplayStatics::OpenLevel(World, *LevelName);
	}

}

void UMultiplayerSessionsSubsystem::JoinCurrentLobbyListenServer()
{
	FSteamId CurrentLobbyOwner = USteamMatchmaking::GetLobbyOwner(LastLobbyJoined);

	if (CurrentLobbyOwner.Result != 0)
	{
		JoinListenServer(CurrentLobbyOwner);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Lobby Owner. Either the lobby owner data was invalid, or the local player is not in a Steam lobby.\n Last Lobby Joined: %llu"), LastLobbyJoined.Result)
	}
}
#pragma endregion