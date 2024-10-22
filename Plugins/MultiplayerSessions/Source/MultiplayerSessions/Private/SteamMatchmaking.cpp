// Fill out your copyright notice in the Description page of Project Settings.


#include "SteamMatchmaking.h"

int32 USteamMatchmaking::AddFavoriteGame(FSteamAppId AppID, FString IP, int32 ConnPort, int32 QueryPort, TArray<int32> Flags, int32 Time32LastPlayedOnServer)
{
	return int32();
}

void USteamMatchmaking::AddRequestLobbyListCompatibleMembersFilter(FSteamId SteamID)
{
	if (!SteamMatchmaking())
	{
		return;
	}

	SteamMatchmaking()->AddRequestLobbyListCompatibleMembersFilter(SteamID.GetSteamID());
}

void USteamMatchmaking::AddRequestLobbyListDistanceFilter(TEnumAsByte<ESteamLobbyDistanceFilter> LobbyDistanceFilter)
{
	if (!SteamMatchmaking())
	{
		return;
	}

	SteamMatchmaking()->AddRequestLobbyListDistanceFilter(static_cast<ELobbyDistanceFilter>(LobbyDistanceFilter.GetValue()));
}

void USteamMatchmaking::AddRequestLobbyListFilterSlotsAvailable(int32 SlotsAvailable)
{
	if (!SteamMatchmaking())
	{
		return;
	}

	SteamMatchmaking()->AddRequestLobbyListFilterSlotsAvailable(SlotsAvailable);
}

void USteamMatchmaking::AddRequestLobbyListNearValueFilter(FString KeyToMatch, int32 ValueToBeCloseTo)
{
	if (!SteamMatchmaking())
	{
		return;
	}

	SteamMatchmaking()->AddRequestLobbyListNearValueFilter(TCHAR_TO_ANSI(*KeyToMatch), ValueToBeCloseTo);
}

void USteamMatchmaking::AddRequestLobbyListNumericalFilter(FString KeyToMatch, int32 ValueToMatch, TEnumAsByte<ESteamLobbyComparison> ComparisonType)
{
	if (!SteamMatchmaking())
	{
		return;
	}

	SteamMatchmaking()->AddRequestLobbyListNumericalFilter(TCHAR_TO_ANSI(*KeyToMatch), ValueToMatch, static_cast<ELobbyComparison>(ComparisonType.GetValue()));
}

void USteamMatchmaking::AddRequestLobbyListResultCountFilter(int32 MaxResults)
{
	if (!SteamMatchmaking())
	{
		return;
	}

	SteamMatchmaking()->AddRequestLobbyListResultCountFilter(MaxResults);
}

void USteamMatchmaking::AddRequestLobbyListStringFilter(FString KeyToMatch, FString ValueToMatch, TEnumAsByte<ESteamLobbyComparison> ComparisonType)
{
	if (!SteamMatchmaking())
	{
		return;
	}

	SteamMatchmaking()->AddRequestLobbyListStringFilter(TCHAR_TO_ANSI(*KeyToMatch), TCHAR_TO_ANSI(*ValueToMatch), static_cast<ELobbyComparison>(ComparisonType.GetValue()));
}

bool USteamMatchmaking::DeleteLobbyData(FSteamId SteamID, FString Key)
{
	return false;
}

bool USteamMatchmaking::GetFavoriteGame(int32 GameIndex, FSteamAppId& AppID, FString& IP, int32& ConnPort, int32& QueryPort, TArray<int32>& Flags, int32& Time32LastPlayedOnServer)
{
	return false;
}

int32 USteamMatchmaking::GetFavoriteGameCount()
{
	return int32();
}

FSteamId USteamMatchmaking::GetLobbyByIndex(int32 LobbyIndex)
{
	return FSteamId();
}

void USteamMatchmaking::GetLobbyChatEntry(FSteamId SteamID, int32 ChatID, FSteamId& SteamIDUser, FString& ChatEntry, TEnumAsByte<ESteamLobbyChatEntryType>& ChatEntryType)
{
}

FString USteamMatchmaking::GetLobbyData(FSteamId LobbyID, FString Key)
{
	if (!SteamMatchmaking())
	{
		return TEXT("");
	}

	return FString(SteamMatchmaking()->GetLobbyData(LobbyID.GetSteamID(), TCHAR_TO_ANSI(*Key)));
}

bool USteamMatchmaking::GetLobbyDataByIndex(FSteamId LobbyID, int32 DataIndex, FString& Key, FString& Value)
{
	if (!SteamMatchmaking())
	{
		return false;
	}

	/* https://partner.steamgames.com/doc/api/ISteamMatchmaking#k_nMaxLobbyKeyLength */
	char key[k_nMaxLobbyKeyLength]; // Key from key/value pair will be output to this array
	char value[k_nMaxLobbyKeyLength]; // Value from key/value pair will be output to this array
	bool result = SteamMatchmaking()->GetLobbyDataByIndex(LobbyID.GetSteamID(), DataIndex, key, k_nMaxLobbyKeyLength, value, k_nMaxLobbyKeyLength);
	Key = FString(key); // Change value of Key by reference
	Value = FString(value); // Change value of Value by reference
	return result;
}

int32 USteamMatchmaking::GetLobbyDataCount(FSteamId LobbyID)
{
	if (!SteamMatchmaking())
	{
		return -1;
	}

	return SteamMatchmaking()->GetLobbyDataCount(LobbyID.GetSteamID());
}

bool USteamMatchmaking::GetLobbyGameServer(FSteamId LobbyID, FString& ServerIP, int32& ServerPort, FSteamId& SteamID)
{
	return false;
}

FSteamId USteamMatchmaking::GetLobbyMemberByIndex(FSteamId LobbyID, int32 MemberIndex)
{
	if (!SteamMatchmaking())
	{
		return FSteamId(0);
	}

	return SteamMatchmaking()->GetLobbyMemberByIndex(LobbyID.GetSteamID(), MemberIndex);
}

FString USteamMatchmaking::GetLobbyMemberData(FSteamId LobbyID, FSteamId UserID, FString Key)
{
	return FString();
}

int32 USteamMatchmaking::GetLobbyMemberLimit(FSteamId LobbyID)
{
	if (!SteamMatchmaking())
	{
		return -1;
	}
	
	return SteamMatchmaking()->GetLobbyMemberLimit(LobbyID.GetSteamID());
}

FSteamId USteamMatchmaking::GetLobbyOwner(FSteamId LobbyID)
{
	return FSteamId();
}

int32 USteamMatchmaking::GetNumLobbyMembers(FSteamId LobbyID)
{
	if (!SteamMatchmaking())
	{
		return 0;
	}

	return SteamMatchmaking()->GetNumLobbyMembers(LobbyID.GetSteamID());
}

bool USteamMatchmaking::InviteUserToLobby(FSteamId LobbyID, FSteamId SteamID)
{
	return false;
}

void USteamMatchmaking::LeaveLobby(FSteamId LobbyID)
{
	if (!SteamMatchmaking())
	{
		return;
	}

	SteamMatchmaking()->LeaveLobby(LobbyID.GetSteamID());
}

bool USteamMatchmaking::RemoveFavoriteGame(FSteamAppId AppID, FString IP, int32 ConnPort, int32 QueryPort, TArray<int32> Flags)
{
	return false;
}

bool USteamMatchmaking::RequestLobbyData(FSteamId LobbyID)
{
	if (!SteamMatchmaking())
	{
		return false;
	}

	SteamMatchmaking()->RequestLobbyData(LobbyID.GetSteamID());
}

bool USteamMatchmaking::SendLobbyChatMessage(FSteamId LobbyID, FString Message)
{
	return false;
}

void USteamMatchmaking::SetLinkedLobby(FSteamId LobbyID, FSteamId DependentLobbyID)
{
}

bool USteamMatchmaking::SetLobbyData(FSteamId LobbyID, FString Key, FString Value)
{
	if (!SteamMatchmaking())
	{
		return false;
	}

	SteamMatchmaking()->SetLobbyData(LobbyID.GetSteamID(), TCHAR_TO_ANSI(*Key), TCHAR_TO_ANSI(*Value));
}

void USteamMatchmaking::SetLobbyGameServer(FSteamId LobbyID, FString ServerIP, int32 ServerPort, FSteamId SteamID)
{
}

bool USteamMatchmaking::SetLobbyJoinable(FSteamId LobbyID, bool bJoinable)
{
	return false;
}

void USteamMatchmaking::SetLobbyMemberData(FSteamId LobbyID, FString Key, FString Value)
{
}

void USteamMatchmaking::SetLobbyMemberLimit(FSteamId LobbyID, int32 MemberLimit)
{
}

void USteamMatchmaking::SetLobbyOwner(FSteamId LobbyID, FSteamId SteamID)
{
}

bool USteamMatchmaking::SetLobbyType(FSteamId LobbyID, TEnumAsByte<ESteamLobbyType> LobbyType)
{
	return false;
}
