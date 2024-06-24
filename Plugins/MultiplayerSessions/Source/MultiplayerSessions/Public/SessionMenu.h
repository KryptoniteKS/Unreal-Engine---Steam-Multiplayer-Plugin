// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "Menu.h"

#include "SessionMenu.generated.h"

UCLASS()
class MULTIPLAYERSESSIONS_API USessionMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void AddItemToList(UObject* SessionItem);
	UFUNCTION(BlueprintCallable)
	void MenuSetup(UMenu* InMenu);

	UPROPERTY(meta = (BindWidget))
	UListView* List;

private:
	UPROPERTY(EditAnywhere, Category = "Session Menu")
	class UMenu* Menu;
};
