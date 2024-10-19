// Fill out your copyright notice in the Description page of Project Settings.

#include "SteamRequestLobbyListAsync.h"

USteamRequestLobbyListAsync* USteamRequestLobbyListAsync::RequestLobbyList()
{
	// Causes Activate() to trigger
	USteamRequestLobbyListAsync* LobbyRequest = NewObject<USteamRequestLobbyListAsync>();
	return LobbyRequest; 
}

void USteamRequestLobbyListAsync::OnLobbyListReceived(LobbyMatchList_t* LobbyMatches, bool bIOFailure)
{
	auto Result = *LobbyMatches;
	AsyncTask(ENamedThreads::GameThread, [this, Result, bIOFailure]()
	{
		if (bIOFailure)
		{
			OnFailure.Broadcast(-1);
		}
		else
		{
			// Broadcast number of lobbies found - we must iterate over these with SteamMatchmaking()->GetLobbyByIndex()
			OnSuccess.Broadcast(Result.m_nLobbiesMatching);
		}
	});

	SetReadyToDestroy();
	MarkAsGarbage();
}

void USteamRequestLobbyListAsync::Activate()
{
	Super::Activate();

	// If we are not connected to SteamMatchmaking, broadcast failure
	if (!SteamMatchmaking())
	{
		OnFailure.Broadcast(-1);
		SetReadyToDestroy();
		MarkAsGarbage();
		return;
	}

	// Perform API call to find lobbies
	RequestLobbyCallbackHandle = SteamMatchmaking()->RequestLobbyList();

	// If API call is invalid, broadcast failure
	if (RequestLobbyCallbackHandle == k_uAPICallInvalid)
	{
		OnFailure.Broadcast(-1);
		SetReadyToDestroy();
		MarkAsGarbage();
		return;
	}
	
	// Add our function to the Steam API callback list
	RequestLobbyCallResult.Set(RequestLobbyCallbackHandle, this, &ThisClass::OnLobbyListReceived);
}
