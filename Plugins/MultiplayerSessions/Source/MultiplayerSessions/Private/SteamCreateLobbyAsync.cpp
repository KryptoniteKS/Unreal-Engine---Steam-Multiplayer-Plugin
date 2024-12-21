// Fill out your copyright notice in the Description page of Project Settings.

#include "SteamCreateLobbyAsync.h"

void USteamCreateLobbyAsync::OnLobbyCreated(LobbyCreated_t* LobbyCreated, bool bIOFailure)
{
	UE_LOG(LogTemp, Warning, TEXT("Steam API successfully returned for CreateLobby. Broadcasting result..."));
	auto Result = *LobbyCreated;
}

/* https://partner.steamgames.com/doc/api/ISteamMatchmaking#LobbyCreated_t */
void USteamCreateLobbyAsync::OnCreateLobbyCallback(LobbyCreated_t* LobbyCreated, bool bIOFailure)
{
	UE_LOG(LogTemp, Warning, TEXT("Steam API successfully returned for CreateLobby. Broadcasting result..."));

	auto LobbyResult = *LobbyCreated;

	AsyncTask(ENamedThreads::GameThread, [this, LobbyResult, bIOFailure]()
	{
	    // If IO failure, broadcast that result
	    if (bIOFailure)
	    {
			UE_LOG(LogTemp, Warning, TEXT("IO Failure in Steam API Call to Create Lobby!"));
	        OnFailure.Broadcast(ESteamResult::ResultIOFailure, FSteamId(0));
	    }
	    else
	    {
	        // Instantiate our result to prep for broadcasting it
	        TEnumAsByte<ESteamResult> Result = static_cast<ESteamResult>(LobbyResult.m_eResult);

	        // If Result is OK, broadcast it. Steam will automatically join us to the lobby in this case
	        if (LobbyResult.m_eResult == k_EResultOK)
	        {
	            OnSuccess.Broadcast(Result, FSteamId(LobbyResult.m_ulSteamIDLobby));
	        }
	        else
	        {
	            OnFailure.Broadcast(Result, FSteamId(LobbyResult.m_ulSteamIDLobby));
	        }
	    }
	});
}

void USteamCreateLobbyAsync::CreateLobby(int32 MaxNumPlayers, TEnumAsByte<ESteamLobbyType> LobbyType)
{
	// If we cannot communicate with Steam Matchmaking, broadcast a failure
	if (!SteamMatchmaking())
	{
		OnFailure.Broadcast(ESteamResult::ResultFail, FSteamId(0));
		return;
	}

	LobbyMaxPlayers = MaxNumPlayers;
	SteamLobbyType = LobbyType;

	UE_LOG(LogTemp, Warning, TEXT("Calling CreateLobby on the Steam API..."));

	// Call the Create Lobby function on Steam API

	CreateLobbyCallbackHandle = SteamMatchmaking()->CreateLobby(ELobbyType::k_ELobbyTypePublic, LobbyMaxPlayers);

	// If the API call was invalid for any reason, broadcast a failure
	if (CreateLobbyCallbackHandle == k_uAPICallInvalid)
	{
		OnFailure.Broadcast(ESteamResult::ResultFail, FSteamId(0));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Setting our callback for CreateLobby on Steam API..."));

	// If all succeeds, bind our callback function to Steam's call result. This ensures OnCreateLobbyCallback() is called and its LobbyCreated_t parameter is populated
	CreateLobbyCallResult.Set(CreateLobbyCallbackHandle, this, &ThisClass::OnCreateLobbyCallback);

	UE_LOG(LogTemp, Warning, TEXT("Successfully set our callback."));
}
