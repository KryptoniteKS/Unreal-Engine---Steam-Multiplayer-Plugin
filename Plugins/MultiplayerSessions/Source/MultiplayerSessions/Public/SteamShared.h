// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SteamShared.generated.h"

/* https://partner.steamgames.com/doc/api/ISteamMatchmaking#ELobbyType */
UENUM(BlueprintType)
enum ELobbyType
{
	LobbyTypePrivate = 0 UMETA(DisplayName = "Private"),
	LobbyTypeFriendsOnly = 1 UMETA(DisplayName = "Friends Only"),
	LobbyTypePublic = 2 UMETA(DisplayName = "Public"),
	LobbyTypeInvisible = 3 UMETA(DisplayName = "Invisible"),
};

/* https://partner.steamgames.com/doc/api/steam_api#CSteamID */
USTRUCT(BlueprintType)
struct FSteamId
{
	GENERATED_BODY()

	UPROPERTY()
	int64 Result = 0;

	CSteamID GetSteamID() const
	{
		CSteamID SteamID;
		SteamID.SetFromUint64(Result);
		return SteamID;
	}
	FSteamId()
	{
		Result = 0;
	}
	FSteamId(int64 SteamID)
	{
		Result = SteamID;
	}

	FSteamId(CSteamID SteamID)
	{
		Result = SteamID.ConvertToUint64();
	}

	FSteamId(FString SteamID)
	{
		Result = FCString::Atoi64(*SteamID);
	}

	CSteamID GetSteamID()
	{
		CSteamID SteamID;
		SteamID.SetFromUint64(Result);
		return SteamID;
	}
};


UCLASS()
class MULTIPLAYERSESSIONS_API USteamShared : public UObject
{
	GENERATED_BODY()
	
};
