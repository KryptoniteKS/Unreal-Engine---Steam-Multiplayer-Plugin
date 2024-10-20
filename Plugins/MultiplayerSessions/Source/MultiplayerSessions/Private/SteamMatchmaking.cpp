// Fill out your copyright notice in the Description page of Project Settings.


#include "SteamMatchmaking.h"

int32 USteamMatchmaking::AddFavoriteGame(FSteamAppId AppID, FString IP, int32 ConnPort, int32 QueryPort, TArray<int32> Flags, int32 Time32LastPlayedOnServer)
{
	return int32();
}

void USteamMatchmaking::AddRequestLobbyListCompatibleMembersFilter(FSteamId SteamID)
{
}

void USteamMatchmaking::AddRequestLobbyListDistanceFilter(TEnumAsByte<ESteamLobbyDistanceFilter> LobbyDistanceFilter)
{
}

void USteamMatchmaking::AddRequestLobbyListFilterSlotsAvailable(int32 SlotsAvailable)
{
}

void USteamMatchmaking::AddRequestLobbyListNearValueFilter(FString KeyToMatch, int32 ValueToBeCloseTo)
{
}

void USteamMatchmaking::AddRequestLobbyListNumericalFilter(FString KeyToMatch, int32 ValueToMatch, TEnumAsByte<ESteamLobbyComparisonType> ComparisonType)
{
}

void USteamMatchmaking::AddRequestLobbyListResultCountFilter(int32 MaxResults)
{
}

void USteamMatchmaking::AddRequestLobbyListStringFilter(FString KeyToMatch, FString ValueToMatch, TEnumAsByte<ESteamLobbyComparisonType> ComparisonType)
{
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
	return FString();
}

bool USteamMatchmaking::GetLobbyDataByIndex(FSteamId LobbyID, int32 DataIndex, FString& Key, FString& Value)
{
	return false;
}

int32 USteamMatchmaking::GetLobbyDataCount(FSteamId LobbyID)
{
	return int32();
}

bool USteamMatchmaking::GetLobbyGameServer(FSteamId LobbyID, FString& ServerIP, int32& ServerPort, FSteamId& SteamID)
{
	return false;
}

FSteamId USteamMatchmaking::GetLobbyMemberByIndex(FSteamId LobbyID, int32 MemberIndex)
{
	return FSteamId();
}

FString USteamMatchmaking::GetLobbyMemberData(FSteamId LobbyID, FSteamId UserID, FString Key)
{
	return FString();
}

int32 USteamMatchmaking::GetLobbyMemberLimit(FSteamId LobbyID)
{
	return int32();
}

FSteamId USteamMatchmaking::GetLobbyOwner(FSteamId LobbyID)
{
	return FSteamId();
}

int32 USteamMatchmaking::GetNumLobbyMembers(FSteamId LobbyID)
{
	return int32();
}

bool USteamMatchmaking::InviteUserToLobby(FSteamId LobbyID, FSteamId SteamID)
{
	return false;
}

void USteamMatchmaking::LeaveLobby(FSteamId LobbyID)
{
}

bool USteamMatchmaking::RemoveFavoriteGame(FSteamAppId AppID, FString IP, int32 ConnPort, int32 QueryPort, TArray<int32> Flags)
{
	return false;
}

bool USteamMatchmaking::RequestLobbyData(FSteamId LobbyID)
{
	return false;
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
	return false;
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
