// Fill out your copyright notice in the Description page of Project Settings.


#include "HostMenu.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/EditableTextBox.h"
#include "AssetRegistry/AssetRegistryModule.h"
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

			MultiplayerSessionsSubsystem->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::OnCreateSession);
		}
	}

	// Fill our Maps dropdown with all maps in the Content/Maps folder and their corresponding thumbnail
	FillMapsDropdown();

	return true;
}

/* Fills the Combo_Maps with all .umap assets that exist in the {ServerMapsDirectory} directory. */
void UHostMenu::FillMapsDropdown()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> AssetDataList;

	// Filter for map assets in the [ServerMapsDirectory]
	FARFilter Filter;
	Filter.ClassNames.Add(UWorld::StaticClass()->GetFName());
	FString MapsPath = FString::Printf(TEXT("/Game/%s"), *ServerMapsDirectory);
	Filter.PackagePaths.Add(FName(*MapsPath));
	Filter.bRecursivePaths = true;

	AssetRegistryModule.Get().GetAssets(Filter, AssetDataList);

	for (const FAssetData& AssetData : AssetDataList)
	{
		FString CurMapName = AssetData.AssetName.ToString();
		ServerMaps.AddUnique(CurMapName);
		Combo_Maps->AddOption(FormatMapName(CurMapName, false));
	}

	if (Combo_Maps->GetOptionCount() <= 0)
	{
		// No options were added, warn the player
		FString Message = FString::Format(TEXT("Please add maps to Content/{0} Directory"), { ServerMapsDirectory });
		Combo_Maps->AddOption(Message);
		HostButton->SetIsEnabled(false); // Keep Host Button disabled, as there are no valid maps to host
	}

	Combo_Maps->SetSelectedIndex(0);

}

void UHostMenu::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Yellow,
				FString(TEXT("Session created Successfully. Traveling to lobby..."))
			);
		}

		UWorld* World = GetWorld();
		if (World)
		{
			// Build our map path and travel to it as a listen server
			auto PathToLobby = FString::Printf(TEXT("/Game/%s/%s?listen"), *ServerMapsDirectory, *FormatMapName(MapName, true));
			World->ServerTravel(PathToLobby);
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Red,
				FString(TEXT("Failed to create session."))
			);
		}

		EnableControls(true);
	}
}

/* Format map name by preserving its literal name or by replacing underscores with whitespace for readability. Note that this enforces a particular naming scheme for Server Maps to display spaces properly. */
FString UHostMenu::FormatMapName(const FString& OriginalName, bool bReturnLiteralName)
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

void UHostMenu::EnableControls(bool bShouldEnable)
{
	// Enable/Disable all controls on the page - Disable when creating a session, enable when failing to create a session
	BackButton->SetIsEnabled(bShouldEnable);
	HostButton->SetIsEnabled(bShouldEnable);
	Text_LobbyName->SetIsEnabled(bShouldEnable);
	Text_NumPlayers->SetIsEnabled(bShouldEnable);
	Combo_Maps->SetIsEnabled(bShouldEnable);
	Combo_GameMode->SetIsEnabled(bShouldEnable);
}

/* Callback function for when the Back button is clicked. Broadcasts to all bound delegate callbacks. */
void UHostMenu::BackButtonClicked()
{
	OnBackButtonClickedDelegate.Broadcast();
}

/* Callback function for when the Host button is clicked. Broadcasts to all bound delegate callbacks with information about the server to host. */
void UHostMenu::HostButtonClicked()
{
	// Grab our data from the controls
	MapName = Combo_Maps->GetSelectedOption();
	LobbyName = Text_LobbyName->GetText().ToString();
	GameMode = Combo_GameMode->GetSelectedOption();
	MaxNumPlayers = FCString::Atoi(*Text_NumPlayers->GetText().ToString()); // Convert Text to FString to CString to Int32

	GEngine->AddOnScreenDebugMessage(
		-1,
		15.f,
		FColor::Emerald,
		FString::Format(TEXT("Creating Session...\nMap: {0}\nLobby: {1}\nGame Mode: {2}\nMax # Players: {3}"), { MapName, LobbyName, GameMode, MaxNumPlayers })
	);

	if (MultiplayerSessionsSubsystem)
	{
		EnableControls(false); // Disable controls to prevent overloading the subsystem with session requests
		MultiplayerSessionsSubsystem->CreateSession(MapName, LobbyName, GameMode, MaxNumPlayers);
	}
}

