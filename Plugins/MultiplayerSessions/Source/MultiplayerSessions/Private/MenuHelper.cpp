// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuHelper.h"
#include "Components/ComboBoxString.h"
#include "AssetRegistry/AssetRegistryModule.h"

/* Initialize Global Variables */
const FString MenuHelper::ServerMapsDirectory = TEXT("Maps/Server");
const TArray<FString> MenuHelper::GameModes = {
	TEXT("Free For All"),
	TEXT("Capture the Flag"),
	TEXT("King of the Hill"),
	TEXT("Team Deathmatch")
};
const FString MenuHelper::AllOption = TEXT("<All>");

/* Populates a game mode combo box with all game modes defined in the MenuHelper class.
Optionally includes an option for "<All>". */
void MenuHelper::PopulateGameModeComboBox(UComboBoxString* Combo_GameModes, bool bIncludeAllOption)
{
	if (bIncludeAllOption) 
	{
		Combo_GameModes->AddOption(AllOption);
	}

	for (const FString& GameMode : GameModes)
	{
		Combo_GameModes->AddOption(GameMode);
	}
	
	if (GameModes.Num() > 0)
	{
		// Set default option in the dropdown to avoid submitting null/empty data
		Combo_GameModes->SetSelectedIndex(0);
	}
}

/* Populates a map combo box with all available maps in the [ServerMapsDirectory].
Optionally includes an option for "<All>". */
bool MenuHelper::PopulateMapComboBox(UComboBoxString* Combo_Maps, bool bIncludeAllOption)
{
	if (bIncludeAllOption)
	{
		Combo_Maps->AddOption(AllOption);
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetDataList;

	// Filter for .umap assets in the [ServerMapsDirectory]
	FARFilter Filter;
	Filter.ClassPaths.Add(UWorld::StaticClass()->GetClassPathName());
	FString MapsPath = FString::Printf(TEXT("/Game/%s"), *ServerMapsDirectory);
	Filter.PackagePaths.Add(FName(*MapsPath));
	Filter.bRecursivePaths = true;

	AssetRegistryModule.Get().GetAssets(Filter, AssetDataList);

	for (const FAssetData& AssetData : AssetDataList)
	{
		FString CurMapName = AssetData.AssetName.ToString();
		//ServerMaps.AddUnique(CurMapName);
		Combo_Maps->AddOption(FormatMapName(CurMapName, false));
	}

	if (Combo_Maps->GetOptionCount() <= 0)
	{
		// No options were added, warn the player
		FString Message = FString::Format(TEXT("Please add maps to Content/{0} Directory"), { ServerMapsDirectory });
		Combo_Maps->AddOption(Message);
		Combo_Maps->SetSelectedIndex(0);
		return false; // Menu will use this to know whether any maps were found. We want to disable hosting if this is the case
		
		//HostButton->SetIsEnabled(false); // Keep Host Button disabled, as there are no valid maps to host
	}

	Combo_Maps->SetSelectedIndex(0);
	return true;
}

/* Format map name by preserving its literal name or by replacing underscores with whitespace for readability.
Note that this enforces a particular naming scheme for Server Maps to display spaces properly. */
FString MenuHelper::FormatMapName(const FString& OriginalName, bool bReturnLiteralName)
{
	FString FormattedName = OriginalName;
	if (bReturnLiteralName)
	{
		return FormattedName.Replace(TEXT(" "), TEXT("_"));
	}
	else
	{
		return FormattedName.Replace(TEXT("_"), TEXT(" "));
	}
}

FString MenuHelper::GetAllOption()
{
	return AllOption;
}
