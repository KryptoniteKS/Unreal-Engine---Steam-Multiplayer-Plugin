// Fill out your copyright notice in the Description page of Project Settings.


#include "SteamJoinLobbyAsync.h"

void USteamJoinLobbyAsync::JoinLobby(FSteamId SteamLobbyId)
{
	// If we cannot communicate with matchmaking API, broadcast a failure
	if (!SteamMatchmaking())
	{
		OnFailure.Broadcast(FSteamId(0), false, ESteamChatRoomEnterResponse::ChatRoomEnterResponseError);
		return;
	}

	// Call JoinLobby on the Steam API
	CallbackHandle = SteamMatchmaking()->JoinLobby(SteamLobbyId.GetSteamID());

	// If the API call is invalid, broadcast a failure
	if (CallbackHandle == k_uAPICallInvalid)
	{
		OnFailure.Broadcast(FSteamId(0), false, ESteamChatRoomEnterResponse::ChatRoomEnterResponseError);
		return;
	}

	CallResult.Set(CallbackHandle, this, &ThisClass::OnLobbyEnter);
}

void USteamJoinLobbyAsync::OnLobbyEnter(LobbyEnter_t* LobbyEnter, bool bIOFailure)
{
	auto Result = *LobbyEnter;
	AsyncTask(ENamedThreads::GameThread, [this, Result, bIOFailure]()
	{
		if (bIOFailure)
		{
		 OnFailure.Broadcast(FSteamId(0), false, ESteamChatRoomEnterResponse::ChatRoomEnterResponseError);
		}
		else
		{
		 TEnumAsByte<ESteamChatRoomEnterResponse> ChatRoomEnterResponse = static_cast<ESteamChatRoomEnterResponse>(Result.m_EChatRoomEnterResponse);
		 FSteamId SteamLobbyId = FSteamId(Result.m_ulSteamIDLobby);
		 if (Result.m_EChatRoomEnterResponse == k_EChatRoomEnterResponseSuccess)
		 {
			 OnSuccess.Broadcast(SteamLobbyId, Result.m_bLocked, ChatRoomEnterResponse);
		 }
		 else
		 {
			  OnFailure.Broadcast(FSteamId(0), false, ChatRoomEnterResponse);
		 }
		}
	});
}
