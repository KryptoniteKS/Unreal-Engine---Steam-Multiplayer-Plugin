// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"

#include "SessionEntry.generated.h"

USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Session Entry")
	FString MapName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Session Entry")
	FString LobbyName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Session Entry")
	FString HostName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Session Entry")
	FString NumPlayers;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Session Entry")
	int32 Ping;

	FSessionInfo():
		MapName(TEXT("Default Map")),
		LobbyName(TEXT("Herobrine's Lobby")),
		HostName(TEXT("Herobrine")),
		NumPlayers(TEXT("0/0")),
		Ping(9999)
	{
	}
};

UCLASS()
class MULTIPLAYERSESSIONS_API USessionEntry : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Session Entry")
	FSessionInfo SessionDetails;

protected:
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

};
