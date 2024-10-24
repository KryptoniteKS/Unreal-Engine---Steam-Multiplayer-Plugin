// Fill out your copyright notice in the Description page of Project Settings.

#include "SteamRequestLobbyListAsync.h"
#include "SteamMatchmaking.h"

USteamRequestLobbyListAsync* USteamRequestLobbyListAsync::RequestLobbyList()
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		15.f,
		FColor::Orange,
		FString(TEXT("Call was made to RequestLobbyListAsync."))
	);
	// Causes Activate() to trigger
	USteamRequestLobbyListAsync* LobbyRequest = NewObject<USteamRequestLobbyListAsync>();
	return LobbyRequest; 
}

void USteamRequestLobbyListAsync::OnLobbyListReceived(LobbyMatchList_t* LobbyMatches, bool bIOFailure)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		15.f,
		FColor::Red,
		FString(TEXT("Received result from Steam API."))
	);

	auto Result = *LobbyMatches;
	AsyncTask(ENamedThreads::GameThread, [this, Result, bIOFailure]()
	{
		if (bIOFailure)
		{
			OnFailure.Broadcast(-1);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Successfully returned lobbies from Steam API. Broadcasting..."))
			);

			// Broadcast number of lobbies found - we must iterate over these with SteamMatchmaking()->GetLobbyByIndex()
			OnSuccess.Broadcast(Result.m_nLobbiesMatching);
		}
	});

	SetReadyToDestroy();
	MarkAsGarbage();
}

void USteamRequestLobbyListAsync::Activate()
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		15.f,
		FColor::Red,
		FString(TEXT("Calling Super::Activate()"))
	);
	Super::Activate();

	// If we are not connected to SteamMatchmaking, broadcast failure
	if (!SteamMatchmaking())
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Red,
			FString(TEXT("SteamMatchmaking was null!"))
		);

		OnFailure.Broadcast(-1);
		SetReadyToDestroy();
		MarkAsGarbage();
		return;
	}

	GEngine->AddOnScreenDebugMessage(
		-1,
		15.f,
		FColor::Red,
		FString(TEXT("Steam Matchmaking was not null! Adding Request Filter..."))
	);
	// Limit search results to first 5000 lobbies
	USteamMatchmaking::AddRequestLobbyListResultCountFilter(5000);

	GEngine->AddOnScreenDebugMessage(
		-1,
		15.f,
		FColor::Red,
		FString(TEXT("Performing API call to Steam."))
	);

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
