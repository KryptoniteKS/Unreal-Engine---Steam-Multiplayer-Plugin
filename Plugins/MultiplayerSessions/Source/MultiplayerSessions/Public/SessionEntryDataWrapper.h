// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SessionEntry.h"
#include "SessionEntryDataWrapper.generated.h"

UCLASS()
class MULTIPLAYERSESSIONS_API USessionEntryDataWrapper : public UObject
{
	GENERATED_BODY()
	
public:
	FSessionInfo SessionInfo;
};
