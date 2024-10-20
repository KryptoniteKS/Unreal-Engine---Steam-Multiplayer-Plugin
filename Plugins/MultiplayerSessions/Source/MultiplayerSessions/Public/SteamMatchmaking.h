// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SteamShared.h"

#include <steam/steam_api.h>
#include <steam/isteammatchmaking.h>
#include <steam/steam_api_common.h>
#include <steam/steamtypes.h>

#include "SteamMatchmaking.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSIONS_API USteamMatchmaking : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/*
	Adds a favorite game to the user's favorites list.
	@param AppID - The AppID of the game.
	@param IP - The IP address of the server.
	@param ConnPort - The connection port of the server.
	@param QueryPort - The query port of the server.
	@param Flags - The flags associated with the favorite.
	@param Time32LastPlayedOnServer - The last time the server was played on.
	@return - The index of the new favorite game.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Add Favorite Game", meta = (Keywords = "AddFavoriteGame"), Category = "Steam Matchmaking")
	static int32 AddFavoriteGame(FSteamAppId AppID, FString IP, int32 ConnPort, int32 QueryPort, TArray<int32> Flags, int32 Time32LastPlayedOnServer);

	/*
	Adds a compatible members filter to the lobby list request.
	@param SteamID - The Steam ID of the user to check compatibility.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Add Request Lobby List Compatible Members Filter", meta = (Keywords = "AddRequestLobbyListCompatibleMembersFilter"), Category = "Steam Matchmaking")
	static void AddRequestLobbyListCompatibleMembersFilter(FSteamId SteamID);

	/*
	Adds a distance filter to the lobby list request.
	@param LobbyDistanceFilter - The distance filter to use.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Add Request Lobby List Distance Filter", meta = (Keywords = "AddRequestLobbyListDistanceFilter"), Category = "Steam Matchmaking")
	static void AddRequestLobbyListDistanceFilter(TEnumAsByte<ESteamLobbyDistanceFilter> LobbyDistanceFilter);

	/*
	Adds a slots available filter to the lobby list request.
	@param SlotsAvailable - The minimum number of slots available.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Add Request Lobby List Filter Slots Available", meta = (Keywords = "AddRequestLobbyListFilterSlotsAvailable"), Category = "Steam Matchmaking")
	static void AddRequestLobbyListFilterSlotsAvailable(int32 SlotsAvailable);

	/*
	Adds a near value filter to the lobby list request.
	@param KeyToMatch - The key to match.
	@param ValueToBeCloseTo - The value to be close to.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Add Request Lobby List Near Value Filter", meta = (Keywords = "AddRequestLobbyListNearValueFilter"), Category = "Steam Matchmaking")
	static void AddRequestLobbyListNearValueFilter(FString KeyToMatch, int32 ValueToBeCloseTo);

	/*
	Adds a numerical filter to the lobby list request.
	@param KeyToMatch - The key to match.
	@param ValueToMatch - The value to match.
	@param ComparisonType - The comparison type to use.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Add Request Lobby List Numerical Filter", meta = (Keywords = "AddRequestLobbyListNumericalFilter"), Category = "Steam Matchmaking")
	static void AddRequestLobbyListNumericalFilter(FString KeyToMatch, int32 ValueToMatch, TEnumAsByte<ESteamLobbyComparisonType> ComparisonType);

	/*
	Adds a result count filter to the lobby list request.
	@param MaxResults - The maximum number of results to return.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Add Request Lobby List Result Count Filter", meta = (Keywords = "AddRequestLobbyListResultCountFilter"), Category = "Steam Matchmaking")
	static void AddRequestLobbyListResultCountFilter(int32 MaxResults);

	/*
	Adds a string filter to the lobby list request.
	@param KeyToMatch - The key to match.
	@param ValueToMatch - The value to match.
	@param ComparisonType - The comparison type to use.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Add Request Lobby List String Filter", meta = (Keywords = "AddRequestLobbyListStringFilter"), Category = "Steam Matchmaking")
	static void AddRequestLobbyListStringFilter(FString KeyToMatch, FString ValueToMatch, TEnumAsByte<ESteamLobbyComparisonType> ComparisonType);

	/*
	Deletes a lobby data key.
	@param SteamID - The Steam ID of the lobby.
	@param Key - The key to delete.
	@return - true if successful, false otherwise.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Delete Lobby Data", meta = (Keywords = "DeleteLobbyData"), Category = "Steam Matchmaking")
	static bool DeleteLobbyData(FSteamId SteamID, FString Key);

	/*
	Gets a favorite game by index.
	@param GameIndex - The index of the game.
	@param AppID - The AppID of the game.
	@param IP - The IP address of the server.
	@param ConnPort - The connection port of the server.
	@param QueryPort - The query port of the server.
	@param Flags - The flags associated with the favorite.
	@param Time32LastPlayedOnServer - The last time the server was played on.
	@return - true if successful, false otherwise.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Get Favorite Game", meta = (Keywords = "GetFavoriteGame"), Category = "Steam Matchmaking")
	static bool GetFavoriteGame(int32 GameIndex, FSteamAppId& AppID, FString& IP, int32& ConnPort, int32& QueryPort, TArray<int32>& Flags, int32& Time32LastPlayedOnServer);

	/*
	Gets the number of favorite games.
	@return - The number of favorite games.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Get Favorite Game Count", meta = (Keywords = "GetFavoriteGameCount"), Category = "Steam Matchmaking")
	static int32 GetFavoriteGameCount();

	/*
	Gets a lobby by index.
	@param LobbyIndex - The index of the lobby.
	@return - The Steam ID of the lobby.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Get Lobby By Index", meta = (Keywords = "GetLobbyByIndex"), Category = "Steam Matchmaking")
	static FSteamId GetLobbyByIndex(int32 LobbyIndex);

	/*
	Gets a chat entry from a lobby.
	@param SteamID - The Steam ID of the lobby.
	@param ChatID - The index of the chat entry.
	@param SteamIDUser - The Steam ID of the user who sent the message.
	@param ChatEntry - The chat entry text.
	@param ChatEntryType - The type of the chat entry.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Get Lobby Chat Entry", meta = (Keywords = "GetLobbyChatEntry"), Category = "Steam Matchmaking")
	static void GetLobbyChatEntry(FSteamId SteamID, int32 ChatID, FSteamId& SteamIDUser, FString& ChatEntry, TEnumAsByte<ESteamLobbyChatEntryType>& ChatEntryType);

	/*
	Gets a lobby data value.
	@param LobbyID - The Steam ID of the lobby.
	@param Key - The key of the data to retrieve.
	@return - The value of the data.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Get Lobby Data", meta = (Keywords = "GetLobbyData"), Category = "Steam Matchmaking")
	static FString GetLobbyData(FSteamId LobbyID, FString Key);

	/*
	Gets a lobby data key-value pair by index.
	@param LobbyID - The Steam ID of the lobby.
	@param DataIndex - The index of the data pair.
	@param Key - The key of the data pair.
	@param Value - The value of the data pair.
	@return - true if successful, false otherwise.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Get Lobby Data By Index", meta = (Keywords = "GetLobbyDataByIndex"), Category = "Steam Matchmaking")
	static bool GetLobbyDataByIndex(FSteamId LobbyID, int32 DataIndex, FString& Key, FString& Value);

	/*
	Gets the number of data pairs in a lobby.
	@param LobbyID - The Steam ID of the lobby.
	@return - The number of data pairs.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Get Lobby Data Count", meta = (Keywords = "GetLobbyDataCount"), Category = "Steam Matchmaking")
	static int32 GetLobbyDataCount(FSteamId LobbyID);

	/*
	Gets the game server information for a lobby.
	@param LobbyID - The Steam ID of the lobby.
	@param ServerIP - The IP address of the game server.
	@param ServerPort - The port of the game server.
	@param SteamID - The Steam ID of the game server.
	@return - true if successful, false otherwise.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Get Lobby Game Server", meta = (Keywords = "GetLobbyGameServer"), Category = "Steam Matchmaking")
	static bool GetLobbyGameServer(FSteamId LobbyID, FString& ServerIP, int32& ServerPort, FSteamId& SteamID);

	/*
	Gets a member of a lobby by index.
	@param LobbyID - The Steam ID of the lobby.
	@param MemberIndex - The index of the member.
	@return - The Steam ID of the member.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Get Lobby Member By Index", meta = (Keywords = "GetLobbyMemberByIndex"), Category = "Steam Matchmaking")
	static FSteamId GetLobbyMemberByIndex(FSteamId LobbyID, int32 MemberIndex);

	/*
	Gets a lobby member's data by key.
	@param LobbyID - The Steam ID of the lobby.
	@param UserID - The Steam ID of the user.
	@param Key - The key of the data to retrieve.
	@return - The value of the data.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Get Lobby Member Data", meta = (Keywords = "GetLobbyMemberData"), Category = "Steam Matchmaking")
	static FString GetLobbyMemberData(FSteamId LobbyID, FSteamId UserID, FString Key);

	/*
	Gets the member limit of a lobby.
	@param LobbyID - The Steam ID of the lobby.
	@return - The member limit of the lobby.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Get Lobby Member Limit", meta = (Keywords = "GetLobbyMemberLimit"), Category = "Steam Matchmaking")
	static int32 GetLobbyMemberLimit(FSteamId LobbyID);

	/*
	Gets the owner of a lobby.
	@param LobbyID - The Steam ID of the lobby.
	@return - The Steam ID of the lobby owner.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Get Lobby Owner", meta = (Keywords = "GetLobbyOwner"), Category = "Steam Matchmaking")
	static FSteamId GetLobbyOwner(FSteamId LobbyID);

	/*
	Gets the number of members in a lobby.
	@param LobbyID - The Steam ID of the lobby.
	@return - The number of members in the lobby.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Get Num Lobby Members", meta = (Keywords = "GetNumLobbyMembers"), Category = "Steam Matchmaking")
	static int32 GetNumLobbyMembers(FSteamId LobbyID);

	/*
	Invites a user to a lobby.
	@param LobbyID - The Steam ID of the lobby.
	@param SteamID - The Steam ID of the user to invite.
	@return - true if successful, false otherwise.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Invite User To Lobby", meta = (Keywords = "InviteUserToLobby"), Category = "Steam Matchmaking")
	static bool InviteUserToLobby(FSteamId LobbyID, FSteamId SteamID);

	/*
	Leaves a lobby.
	@param LobbyID - The Steam ID of the lobby.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Leave Lobby", meta = (Keywords = "LeaveLobby"), Category = "Steam Matchmaking")
	static void LeaveLobby(FSteamId LobbyID);

	/*
	Removes a favorite game from the user's favorites list.
	@param AppID - The AppID of the game.
	@param IP - The IP address of the server.
	@param ConnPort - The connection port of the server.
	@param QueryPort - The query port of the server.
	@param Flags - The flags associated with the favorite.
	@return - true if successful, false otherwise.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Remove Favorite Game", meta = (Keywords = "RemoveFavoriteGame"), Category = "Steam Matchmaking")
	static bool RemoveFavoriteGame(FSteamAppId AppID, FString IP, int32 ConnPort, int32 QueryPort, TArray<int32> Flags);

	/*
	Requests lobby data from the server.
	@param LobbyID - The Steam ID of the lobby.
	@return - true if successful, false otherwise.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Request Lobby Data", meta = (Keywords = "RequestLobbyData"), Category = "Steam Matchmaking")
	static bool RequestLobbyData(FSteamId LobbyID);

	/*
	Sends a chat message to a lobby.
	@param LobbyID - The Steam ID of the lobby.
	@param Message - The message to send.
	@return - true if successful, false otherwise.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Send Lobby Chat Message", meta = (Keywords = "SendLobbyChatMessage"), Category = "Steam Matchmaking")
	static bool SendLobbyChatMessage(FSteamId LobbyID, FString Message);

	/*
	Sets a linked lobby.
	@param LobbyID - The Steam ID of the lobby.
	@param DependentLobbyID - The Steam ID of the dependent lobby.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Set Linked Lobby", meta = (Keywords = "SetLinkedLobby"), Category = "Steam Matchmaking")
	static void SetLinkedLobby(FSteamId LobbyID, FSteamId DependentLobbyID);

	/*
	Sets a lobby data key-value pair.
	@param LobbyID - The Steam ID of the lobby.
	@param Key - The key of the data to set.
	@param Value - The value of the data to set.
	@return - true if successful, false otherwise.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Set Lobby Data", meta = (Keywords = "SetLobbyData"), Category = "Steam Matchmaking")
	static bool SetLobbyData(FSteamId LobbyID, FString Key, FString Value);

	/*
	Sets the game server information for a lobby.
	@param LobbyID - The Steam ID of the lobby.
	@param ServerIP - The IP address of the game server.
	@param ServerPort - The port of the game server.
	@param SteamID - The Steam ID of the game server.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Set Lobby Game Server", meta = (Keywords = "SetLobbyGameServer"), Category = "Steam Matchmaking")
	static void SetLobbyGameServer(FSteamId LobbyID, FString ServerIP, int32 ServerPort, FSteamId SteamID);

	/*
	Sets whether a lobby is joinable.
	@param LobbyID - The Steam ID of the lobby.
	@param bJoinable - true if the lobby is joinable, false otherwise.
	@return - true if successful, false otherwise.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Set Lobby Joinable", meta = (Keywords = "SetLobbyJoinable"), Category = "Steam Matchmaking")
	static bool SetLobbyJoinable(FSteamId LobbyID, bool bJoinable);

	/*
	Sets a lobby member's data key-value pair.
	@param LobbyID - The Steam ID of the lobby.
	@param Key - The key of the data to set.
	@param Value - The value of the data to set.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Set Lobby Member Data", meta = (Keywords = "SetLobbyMemberData"), Category = "Steam Matchmaking")
	static void SetLobbyMemberData(FSteamId LobbyID, FString Key, FString Value);

	/*
	Sets the member limit of a lobby.
	@param LobbyID - The Steam ID of the lobby.
	@param MemberLimit - The new member limit.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Set Lobby Member Limit", meta = (Keywords = "SetLobbyMemberLimit"), Category = "Steam Matchmaking")
	static void SetLobbyMemberLimit(FSteamId LobbyID, int32 MemberLimit);

	/*
	Sets the owner of a lobby.
	@param LobbyID - The Steam ID of the lobby.
	@param SteamID - The Steam ID of the new owner.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Set Lobby Owner", meta = (Keywords = "SetLobbyOwner"), Category = "Steam Matchmaking")
	static void SetLobbyOwner(FSteamId LobbyID, FSteamId SteamID);

	/*
	Sets the type of a lobby.
	@param LobbyID - The Steam ID of the lobby.
	@param LobbyType - The new lobby type.
	@return - true if successful, false otherwise.
	*/
	UFUNCTION(BlueprintCallable, DisplayName = "Set Lobby Type", meta = (Keywords = "SetLobbyType"), Category = "Steam Matchmaking")
	static bool SetLobbyType(FSteamId LobbyID, TEnumAsByte<ESteamLobbyType> LobbyType);
};
