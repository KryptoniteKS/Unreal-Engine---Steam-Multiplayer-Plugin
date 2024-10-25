// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "SteamShared.h"

#include <steam/steam_api.h>
#include <steam/isteammatchmaking.h>
#include <steam/steam_api_common.h>

#include "SteamRequestLobbyListAsync.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRequestLobbyList, int32, LobbiesMatching);

UCLASS()
class MULTIPLAYERSESSIONS_API USteamRequestLobbyListAsync : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintAssignable)
	FOnRequestLobbyList OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FOnRequestLobbyList OnFailure;

	UFUNCTION(BlueprintCallable, DisplayName = "Request Lobby List", meta = (BlueprintInternalUseOnly = "true"), Category = "Steam Matchmaking")
	void RequestLobbyList();

private:
	void OnLobbyListReceived(LobbyMatchList_t* LobbyMatches, bool bIOFailure);

	SteamAPICall_t RequestLobbyCallbackHandle;
	CCallResult<USteamRequestLobbyListAsync, LobbyMatchList_t> RequestLobbyCallResult;

};
