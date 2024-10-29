// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SteamShared.h"

#include <steam/steamtypes.h>
#include <steam/isteammatchmaking.h>
#include <steam/steam_api_common.h>
#include <steam/steam_api.h>

#include "SteamCreateLobbyAsync.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCreateLobbyDelegate, TEnumAsByte<ESteamResult>, Result, FSteamId, LobbyID);

UCLASS()
class MULTIPLAYERSESSIONS_API USteamCreateLobbyAsync : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, DisplayName = "Create Steam Lobby", meta = (BlueprintInternalUseOnly = "true"), Category = "Steam Matchmaking")
	void CreateLobby(int32 MaxNumPlayers, TEnumAsByte<ESteamLobbyType> LobbyType);

	UPROPERTY(BlueprintAssignable)
	FCreateLobbyDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FCreateLobbyDelegate OnFailure;

	void OnLobbyCreated(LobbyCreated_t* LobbyCreated, bool bIOFailure);

private:
	/* Our custom callback function that will be called if Steam's CreateLobby function call is successful. Must be bound using our CCallResult.Set() */
	void OnCreateLobbyCallback(LobbyCreated_t* LobbyCreated, bool bIOFailure);

	int32 LobbyMaxPlayers;
	TEnumAsByte<ESteamLobbyType> SteamLobbyType;
	SteamAPICall_t CreateLobbyCallbackHandle; // Callback handle return type for Steam's CreateLobby() method
	CCallResult<USteamCreateLobbyAsync, LobbyCreated_t> CreateLobbyCallResult; // Essentially a delegate handle we can use to add our callback function to Steam's delegate list






};
