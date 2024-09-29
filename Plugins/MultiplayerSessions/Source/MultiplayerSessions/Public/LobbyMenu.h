// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API ULobbyMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void LobbyMenuSetup();
	virtual bool Initialize();

	UFUNCTION(BlueprintCallable, Category = "Session Management")
	void AddSession(USessionEntry* Session);

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Session Management")
	class USessionEntry* SelectedSession;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Session Management")
	TArray<USessionEntry*> Sessions;


private:
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinButton;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	class UScrollBox* ScrollBox_Sessions;
	UPROPERTY(EditAnywhere)
	TSubclassOf<USessionEntry> SessionEntryClass;

	UFUNCTION()
	void JoinButtonClicked();
	
};
