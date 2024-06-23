// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"

#include "SessionMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API USessionMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void AddItemToList(UObject* SessionItem);

	UPROPERTY(meta = (BindWidget))
	UListView* List;
};
