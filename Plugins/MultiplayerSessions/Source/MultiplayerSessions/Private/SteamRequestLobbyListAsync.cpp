// Fill out your copyright notice in the Description page of Project Settings.

#include "SteamRequestLobbyListAsync.h"
#include "SteamMatchmaking.h"

void USteamRequestLobbyListAsync::OnLobbyListReceived(LobbyMatchList_t* LobbyMatches, bool bIOFailure)
{
	UE_LOG(LogTemp, Warning, TEXT("Received RequestLobbyList result from Steam API."));

	auto Result = *LobbyMatches;
	AsyncTask(ENamedThreads::GameThread, [this, Result, bIOFailure]()
	{
		if (bIOFailure)
		{
			UE_LOG(LogTemp, Error, TEXT("IO Failure detected on receive result from RequestLobbyList."));


			OnFailure.Broadcast(-1);
		}
		else
		{
			// Broadcast number of lobbies found - we must iterate over these with SteamMatchmaking()->GetLobbyByIndex()
			auto numLobbies = static_cast<int32>(Result.m_nLobbiesMatching);

			UE_LOG(LogTemp, Error, TEXT("%d Steam lobbies found!"), numLobbies);

			OnSuccess.Broadcast(numLobbies);
		}
	});
}

void USteamRequestLobbyListAsync::RequestLobbyList()
{
	// If we are not connected to SteamMatchmaking, broadcast failure
	if (!SteamMatchmaking())
	{
		UE_LOG(LogTemp, Warning, TEXT("SteamMatchmaking was null!"));

		OnFailure.Broadcast(-1);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Steam Matchmaking was not null! Adding Request Filter..."));

	// Limit search results to first 5000 lobbies
	USteamMatchmaking::AddRequestLobbyListResultCountFilter(200);
	USteamMatchmaking::AddRequestLobbyListFilterSlotsAvailable(0);
	USteamMatchmaking::AddRequestLobbyListDistanceFilter(ESteamLobbyDistanceFilter::LobbyDistanceFilterFar);

	UE_LOG(LogTemp, Warning, TEXT("Performing API call RequestLobbyList."));


	// Perform API call to find lobbies
	RequestLobbyCallbackHandle = SteamMatchmaking()->RequestLobbyList();

	// If API call is invalid, broadcast failure
	if (RequestLobbyCallbackHandle == k_uAPICallInvalid)
	{
		OnFailure.Broadcast(-1);
		return;
	}
	
	// Add our function to the Steam API callback list
	RequestLobbyCallResult.Set(RequestLobbyCallbackHandle, this, &ThisClass::OnLobbyListReceived);
}
