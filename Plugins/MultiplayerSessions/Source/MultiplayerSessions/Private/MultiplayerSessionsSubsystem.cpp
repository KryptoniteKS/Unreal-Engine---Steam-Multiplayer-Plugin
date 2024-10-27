// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionsSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "SteamRequestLobbyListAsync.h"
#include "SteamCreateLobbyAsync.h"
#include "SteamMatchmaking.h"

UMultiplayerSessionsSubsystem::UMultiplayerSessionsSubsystem():
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
	DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
	StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();
	}

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

	SteamAPI_Init();
}

void UMultiplayerSessionsSubsystem::CreateSession(FString MapName, FString LobbyName, FString GameMode, int32 MaxNumPlayers)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	auto ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
	{
		bCreateSessionOnDestroy = true;
		LastMaxNumPlayers = MaxNumPlayers;
		LastGameMode = GameMode;
		LastMapName = MapName;
		LastLobbyName = LobbyName;

		// Destroy the session if one already exists
		DestroySession();
	}

	// Store the delegate in the FDelegate handle so we can later remove it from the delegate list
	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);
	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	LastSessionSettings->NumPublicConnections = MaxNumPlayers; // max num players allowed in our game
	LastSessionSettings->bAllowJoinInProgress = true; // allow players to join while a session is already running
	LastSessionSettings->bAllowJoinViaPresence = true; // presence means searching for sessions going on in your region of the world
	LastSessionSettings->bShouldAdvertise = true; // allows steam to advertise the session so others can find and join
	LastSessionSettings->bUsesPresence = true; // allows us to use presence in order to find sessions going on in our region
	LastSessionSettings->bUseLobbiesIfAvailable = true; // required for some reason - need more research
	LastSessionSettings->Set(FName(Key_MapName), MapName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing); // set the Map Name key/value pair
	LastSessionSettings->Set(FName(Key_LobbyName), LobbyName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing); // set the Map Name key/value pair
	LastSessionSettings->Set(FName(Key_GameMode), GameMode, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing); // set the Game Mode key/value pair
	LastSessionSettings->BuildUniqueId = 1; // Used to keep different builds from seeing each other during searches

	if (SteamFriends())
	{
		FString hostName = UTF8_TO_TCHAR(SteamFriends()->GetPersonaName());
		LastSessionSettings->Set(FName(Key_HostName), hostName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	}

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (!SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings))
	{
		// If we fail to create a session, remove the delegate from the delegate list using our delegate handle - our callback won't be called anyway
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);

		// Broadcast our own custom delegate. False since we failed to create a session. We can broadcast true in our OnCreateSessionComplete callback function on our subsystem.
		MultiplayerOnCreateSessionComplete.Broadcast(false);
	}
}

void UMultiplayerSessionsSubsystem::FindSessions(int32 MaxSearchResults)
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	// Add our delegate to the delegate list
	FindSessionsCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	LastSessionSearch->TimeoutInSeconds = 30.f;

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController(); // need this to get the FUniqueNetID
	if (!SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{

		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Red,
			FString(TEXT("Find sessions failed to be invoked."))
		);

		// If we fail to start finding sessions, we will clear the delegate and broadcast that it failed.
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false); // We can pass an empty array here since it failed
	}

	// If FindSessions doesn't return false, we can carry out our logic on the callback OnFindSessionsComplete
}

void UMultiplayerSessionsSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
	if (!SessionInterface.IsValid())
	{
		// Broadcast an error to our custom delegate
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		return;
	}

	JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController(); // need this to get the FUniqueNetID
	if (!SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
	{
		// If we fail to begin joining a session, let's clear the delegate and broadcast our failure
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		MultiplayerOnJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
	}

	// If we succeed at calling JoinSession, we can expect our callback OnJoinSessionComplete to be triggered
}

void UMultiplayerSessionsSubsystem::DestroySession()
{
	if (!SessionInterface.IsValid())
	{
		// Broadcast that we were not able to destroy the session then return
		MultiplayerOnDestroySessionComplete.Broadcast(false);
		return;
	}

	DestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	if (!SessionInterface->DestroySession(NAME_GameSession))
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
		MultiplayerOnDestroySessionComplete.Broadcast(false);
	}

	// If DestroySession is successfully called, our callback OnDestroySessionComplete will be triggered.
}

void UMultiplayerSessionsSubsystem::StartSession()
{
}

void UMultiplayerSessionsSubsystem::RequestLobbyList()
{
	SteamRequestLobbyListAsync->RequestLobbyList();
}

void UMultiplayerSessionsSubsystem::OnRequestLobbyList(int32 LobbiesMatching)
{
	if (LobbiesMatching < 0)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Red,
			FString(TEXT("Request Lobby List failed!"))
		);

		return;
	}

	GEngine->AddOnScreenDebugMessage(
		-1,
		15.f,
		FColor::Red,
		FString(TEXT("Looping through lobbies..."))
	);
	// Loop through the sessions by using GetLobbyByIndex
	TArray<FLobbyEntry> LobbyData;
	for (int curLobby = 0; curLobby < LobbiesMatching - 1; curLobby++)
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
		LobbyData.Add(LobbyEntry);
	}

	
	if (LobbyData.Num() > 0)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Red,
			FString(TEXT("At least 1 lobby found."))
		);
	}

	GEngine->AddOnScreenDebugMessage(
		-1,
		15.f,
		FColor::Red,
		FString(TEXT("Broadcasting Lobby Data..."))
	);
	OnRequestLobbyListComplete.Broadcast(LobbyData);
	
}

void UMultiplayerSessionsSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface)
	{
		// Remove our delegate from the delegate list. Our callback (this function) has already been triggered, so no need for this overhead anymore.
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	// Broadcast our custom delegate to our menu class callback function
	MultiplayerOnCreateSessionComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	// Let's clear the delegate first
	if (SessionInterface)
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	// If our array is empty, let's just broadcast false to the menu with an empty array.
	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		MultiplayerOnFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return; // So we don't broadcast again in the below code
	}

	// Then let's broadcast our results to our custom delegate
	MultiplayerOnFindSessionsComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	MultiplayerOnJoinSessionComplete.Broadcast(Result);
}

void UMultiplayerSessionsSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface)
	{
		// Clear the delegate from the delegate list
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}

	if (bWasSuccessful && bCreateSessionOnDestroy)
	{
		// If we have successfully destroyed a session and want to create a new one, let's do it
		bCreateSessionOnDestroy = false; // reset the boolean to prevent bad logic in the future
		CreateSession(LastMapName, LastLobbyName, LastGameMode, LastMaxNumPlayers);
	}

	// Regardless of outcome, let's still broadcast the result
	MultiplayerOnDestroySessionComplete.Broadcast(bWasSuccessful);
}

void UMultiplayerSessionsSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}
