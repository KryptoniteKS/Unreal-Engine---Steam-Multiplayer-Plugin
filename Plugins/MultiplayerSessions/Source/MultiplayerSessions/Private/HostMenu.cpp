// Fill out your copyright notice in the Description page of Project Settings.


#include "HostMenu.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "MultiplayerSessionsSubsystem.h"

bool UHostMenu::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (BackButton)
	{
		BackButton->OnClicked.AddDynamic(this, &ThisClass::BackButtonClicked);
	}
	if (HostButton)
	{
		HostButton->OnClicked.AddDynamic(this, &ThisClass::HostButtonClicked);
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionsSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionsSubsystem>();
		if (MultiplayerSessionsSubsystem)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Yellow,
				FString(TEXT("Multiplayer Session Subsystem successfully initialized on the Host Menu."))
			);
		}
	}

	// Fill our Maps dropdown with all maps in the Content/Maps folder and their corresponding thumbnail
	FillMapsDropdown();

	return true;
}

/* Fills the Combo_Maps with all .umap assets that exist in the {ServerMapsDirectory} directory. */
void UHostMenu::FillMapsDropdown()
{
	FString MapDirectory = FPaths::ProjectContentDir() / ServerMapsDirectory;
	TArray<FString> MapFiles;
	FString FileType = TEXT("*.umap");
	IFileManager& FileManager = IFileManager::Get();

	FileManager.FindFiles(MapFiles, *(MapDirectory / FileType), true, false);


	for (const FString& MapFile : MapFiles)
	{
		auto CurMapName = MapFile.Replace(TEXT(".umap"), TEXT(""), ESearchCase::IgnoreCase);
		ServerMaps.AddUnique(CurMapName);
		Combo_Maps->AddOption(FormatMapName(CurMapName));
	}

}

/* Replaces all occurrences of the underscore character with a space. Note that this enforces a particular naming scheme for Server Maps to display properly. */
FString UHostMenu::FormatMapName(const FString& OriginalName)
{
	FString FormattedName = OriginalName;
	return FormattedName.Replace(TEXT("_"), TEXT(" "));
}

/* Callback function for when the Back button is clicked. Broadcasts to all bound delegate callbacks. */
void UHostMenu::BackButtonClicked()
{
	OnBackButtonClickedDelegate.Broadcast();
}

/* Callback function for when the Host button is clicked. Broadcasts to all bound delegate callbacks with information about the server to host. */
void UHostMenu::HostButtonClicked()
{
	// Broadcast our host data to our subsystem
	OnHostButtonClickedDelegate.Broadcast(MapName, LobbyName, GameMode, MaxNumPlayers);
}

