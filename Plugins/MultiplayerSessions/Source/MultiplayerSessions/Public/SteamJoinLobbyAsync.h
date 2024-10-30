// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SteamShared.h"

#include <steam/steam_api.h>
#include <steam/isteammatchmaking.h>
#include <steam/steam_api_common.h>

#include "SteamJoinLobbyAsync.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnJoinLobby, FSteamId, LobbyId, bool, bLocked, TEnumAsByte<ESteamChatRoomEnterResponse>, ChatRoomEnterResponse);

UCLASS()
class MULTIPLAYERSESSIONS_API USteamJoinLobbyAsync : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, DisplayName = "Join Lobby", meta = (BlueprintInternalUseOnly = "true"), Category = "Steam Matchmaking")
	void JoinLobby(FSteamId SteamLobbyId);

	UPROPERTY(BlueprintAssignable)
	FOnJoinLobby OnSuccess;
	UPROPERTY(BlueprintAssignable)
	FOnJoinLobby OnFailure;

private:
	void OnLobbyEnter(LobbyEnter_t* LobbyEnter, bool bIOFailure);
	SteamAPICall_t CallbackHandle;
	CCallResult<USteamJoinLobbyAsync, LobbyEnter_t> CallResult;
};
