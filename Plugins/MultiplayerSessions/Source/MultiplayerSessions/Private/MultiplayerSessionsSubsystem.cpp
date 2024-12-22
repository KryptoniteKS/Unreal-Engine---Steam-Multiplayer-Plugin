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
	if (SteamAPI_IsSteamRunning())
	{
		UE_LOG(LogTemp, Log, TEXT("Steam is currently running. The Steam API will now be initialized and its functions should be accessible."));
		SteamAPI_Init();
		InitializeSteamCallbacks();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Steam is not running. Steam API functions will not be accessible."));
	}
}

void UMultiplayerSessionsSubsystem::InitializePingLocation()
{
	// Start a timer to check relay network status
	GetWorld()->GetTimerManager().SetTimer(
		RelayNetworkCheckTimerHandle,
		this,
		&ThisClass::CheckRelayNetworkStatus,
		1.f,
		true // Looping timer
	);
}

void UMultiplayerSessionsSubsystem::CheckRelayNetworkStatus()
{
	if (SteamNetworkingUtils())
	{
		SteamRelayNetworkStatus_t RelayNetworkStatusDetails;
		ESteamNetworkingAvailability CurrentStatus = SteamNetworkingUtils()->GetRelayNetworkStatus(&RelayNetworkStatusDetails);

		// Log the current availability status
		UE_LOG(LogTemp, Log, TEXT("Relay Network Status: %d"), static_cast<int32>(CurrentStatus));

		// Check if the relay network is ready
		if (CurrentStatus == k_ESteamNetworkingAvailability_Current)
		{
			UE_LOG(LogTemp, Log, TEXT("Relay network is initialized and ready."));

			LocalPingLocationAge = SteamNetworkingUtils()->GetLocalPingLocation(LocalPingLocation);
			if (LocalPingLocationAge > 0)
			{
				UE_LOG(LogTemp, Log, TEXT("Successfully retrieved local ping location. Age: %f\nClearing timer handle."), LocalPingLocationAge);

				// Clear the timer when the relay network is ready
				GetWorld()->GetTimerManager().ClearTimer(RelayNetworkCheckTimerHandle);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Failed to return Local Ping Location. Retrying..."));
			}
		}
		else
		{
			// Log additional details about the relay network status
			UE_LOG(LogTemp, Log, TEXT("Relay network not available yet. Status details: m_eAvail=%d, m_debugMsg=%s"),
				   static_cast<int32>(RelayNetworkStatusDetails.m_eAvail),
				   UTF8_TO_TCHAR(RelayNetworkStatusDetails.m_debugMsg));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SteamNetworkingUtils is null!"));
	}
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

	if (SteamMatchmaking())
	{
		m_CallbackSetLobbyGameServer.Register(this, &ThisClass::OnSetLobbyGameServerCallback);
	}
}

UMultiplayerSessionsSubsystem::~UMultiplayerSessionsSubsystem()
{
	m_CallbackGameLobbyJoinRequested.Unregister();
	m_CallbackSetLobbyGameServer.Unregister();
}

void UMultiplayerSessionsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Initializes the steam relay network to allow us to make ping estimations
	if (SteamNetworkingUtils())
	{
		SteamNetworkingUtils()->InitRelayNetworkAccess();
		InitializePingLocation();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SteamNetworkingUtils was not yet initialized whilst initializing our MultiplayerSessionsSubsystem!"));
	}

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
			JoinCurrentLobbyGameServer();
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
	LeaveCurrentLobby();

	SteamJoinLobbyAsync->JoinLobby(LobbyID);
	// Should eventually trigger our OnJoinLobby callback with the Steam API response
}

void UMultiplayerSessionsSubsystem::LeaveCurrentLobby()
{
	USteamMatchmaking::LeaveLobby(CurrentLobbyId);
}

void UMultiplayerSessionsSubsystem::SetLobbyGameServer(FSteamId LobbyID, FString ServerIP, int32 ServerPort, FSteamId GameServerID)
{
	// Only need to send Steam ID for Lobby and Listen Server - Will trigger our OnSetLobbyGameServerCallback function if successful
	USteamMatchmaking::SetLobbyGameServer(LobbyID, ServerIP, ServerPort, GameServerID);
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

		if (LocalPingLocationAge <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Local Ping Location Age (%f) indicates it is unitialized! Lobby Metadata will not reflect the correct ping location."), LocalPingLocationAge);
		}
		else
		{
			if (SteamNetworkingUtils())
			{
				char SerializedPingLocation[k_cchMaxSteamNetworkingPingLocationString];
				UE_LOG(LogTemp, Log, TEXT("Attempting to serialize Local Ping Location..."));
				SteamNetworkingUtils()->ConvertPingLocationToString(LocalPingLocation, SerializedPingLocation, sizeof(SerializedPingLocation));

				// Store the serialized ping location as a lobby metadata key
				UE_LOG(LogTemp, Log, TEXT("Setting Lobby Metadata - Key: %s - Value: %hs"), *Key_PingLocation, SerializedPingLocation);
				USteamMatchmaking::SetLobbyData(LobbyID, Key_PingLocation, FString(UTF8_TO_TCHAR(SerializedPingLocation)));
			}
		}

		// Set the lobby's game server. For now it is just a listen server so there is no IP or Port
		if (SteamUser())
		{
			FSteamId MySteamID = FSteamId(SteamUser()->GetSteamID());
			UE_LOG(LogTemp, Warning, TEXT("Setting Lobby Game Server to current player's Steam ID: %llu - "), MySteamID.Result);
			SetLobbyGameServer(LobbyID, TEXT(""), 0, FSteamId(MySteamID));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to set Lobby Game Server. Steam User API was not initialized."));
		}

		OnCreateLobbyComplete.Broadcast(true);
	}
	else
	{
		OnCreateLobbyComplete.Broadcast(false);
	}
}

void UMultiplayerSessionsSubsystem::OnRequestLobbyList(int32 LobbiesMatching)
{
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

		LobbyEntry.Ping = 100;
		FString LobbyPingLocation = USteamMatchmaking::GetLobbyData(LobbyId, Key_PingLocation);
		if (LobbyPingLocation.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("Ping Location for Lobby ID %llu was empty! Ping will show as 100."), LobbyId.Result);
		}
		else
		{
			SteamNetworkPingLocation_t ParsedPingLocation;
			if (SteamNetworkingUtils()->ParsePingLocationString(TCHAR_TO_UTF8(*LobbyPingLocation), ParsedPingLocation))
			{
				int32 EstimatedPing = SteamNetworkingUtils()->EstimatePingTimeFromLocalHost(ParsedPingLocation);

				if (EstimatedPing >= 0)
				{
					UE_LOG(LogTemp, Log, TEXT("Successfully parsed Lobby %llu ping location! Estimated ping is %d"), LobbyId.Result, EstimatedPing);
					LobbyEntry.Ping = EstimatedPing;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Ping estimation failed for Lobby ID %llu. Ping will show as 100."), LobbyId.Result);
				}				
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Ping estimation failed for Lobby ID %llu. Ping will show as 100."), LobbyId.Result);
			}
		}

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
		UE_LOG(LogTemp, Error, TEXT("Steam API SteamChatRoomEnterResponse indicates success."));
		LastLobbyJoined = LobbyId;
		OnJoinLobbyComplete.Broadcast(true); // Triggers the OnJoinLobby function on the LobbyMenu class
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Steam API SteamChatRoomEnterResponse does not indicate success."));
		OnJoinLobbyComplete.Broadcast(false); // Triggers the OnJoinLobby function on the LobbyMenu class
	}
}
#pragma endregion

#pragma region Steam API Registered Callbacks
void UMultiplayerSessionsSubsystem::OnGameLobbyJoinRequestedCallback(GameLobbyJoinRequested_t* pCallback)
{
	UE_LOG(LogTemp, Warning, TEXT("Received GameLobbyJoinRequested callback from Steam. Attempting to connect to requested lobby and associated Game server..."));
	JoinLobby(FSteamId(pCallback->m_steamIDLobby));
	JoinCurrentLobbyGameServer();
}

void UMultiplayerSessionsSubsystem::OnSetLobbyGameServerCallback(LobbyGameCreated_t* pCallback)
{
	UE_LOG(LogTemp, Warning, TEXT("Received SetLobbyGameServer callback from Steam. Joining current lobby's game server."));
	JoinCurrentLobbyGameServer();
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

void UMultiplayerSessionsSubsystem::JoinCurrentLobbyGameServer()
{
	if (!SteamUser())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to join Lobby Game Server. Steam User API was not initialized."));
		return;
	}

	FSteamId CurrentLobbyOwner = USteamMatchmaking::GetLobbyOwner(LastLobbyJoined);

	FString ServerIP;
	int32 ServerPort;
	FSteamId ServerID;
	USteamMatchmaking::GetLobbyGameServer(LastLobbyJoined, ServerIP, ServerPort, ServerID);

	// Ensure there is a valid game server and that it matches the host of the lobby but is NOT yourself (you don't want to request to join your own listen server).
	if (CurrentLobbyOwner.Result != 0 && CurrentLobbyOwner.Result == ServerID.Result && CurrentLobbyOwner.Result != FSteamId(SteamUser()->GetSteamID()).Result)
	{
		UE_LOG(LogTemp, Warning, TEXT("Joining current lobby's associated game server with Server ID: %llu"), ServerID.Result);
		JoinListenServer(ServerID);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Either the Current Lobby Owner was invalid, the associated game server did not match the original owner, or the user attempting to join the listen server was already the host. Current Lobby ID: %llu"), LastLobbyJoined.Result)
	}
}
#pragma endregion