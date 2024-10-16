// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

class MULTIPLAYERSESSIONS_API MenuHelper
{
public:
	static void PopulateGameModeComboBox(class UComboBoxString* Combo_GameModes, bool bIncludeAllOption);
	static bool PopulateMapComboBox(class UComboBoxString* Combo_Maps, bool bIncludeAllOption);
	static FString FormatMapName(const FString& OriginalName, bool bReturnLiteralName);
	static FString GetAllOption();

private:
	static const FString ServerMapsDirectory; // Directory beginning from Game/Content/ directory
	static const TArray<FString> GameModes;
	static const FString AllOption; // String for the 'All' option to be added to combo boxes

};
