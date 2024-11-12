// Fill out your copyright notice in the Description page of Project Settings.


#include "SteamFriends.h"

void USteamFriends::ActivateGameOverlay(const FString& DialogToOpen)
{
}

void USteamFriends::ActivateGameOverlayInviteDialog(const int64& SteamIdLobby)
{
}

void USteamFriends::ActivateGameOverlayToStore(const FSteamAppId& AppID, bool bAddToCartAndShow)
{
}

void USteamFriends::ActivateGameOverlayToUser(const FString& Dialog, int64 SteamIdUser)
{
}

void USteamFriends::ActivateGameOverlayToWebPage(const FString& URL, bool bUseModal)
{
}

void USteamFriends::ClearRichPresence()
{
}

bool USteamFriends::CloseClanChatWindowInSteam(int64 SteamIdClanChat)
{
	return false;
}

FSteamId USteamFriends::GetChatMemberByIndex(FSteamId SteamIdClan, int32 MemberIndex)
{
	return FSteamId();
}

bool USteamFriends::GetClanActivityCounts(FSteamId SteamIdClan, int32& Online, int32& InGame, int32& Chatting)
{
	return false;
}

FSteamId USteamFriends::GetClanByIndex(int32 ClanIndex)
{
	return FSteamId();
}

int32 USteamFriends::GetClanChatMemberCount(FSteamId SteamIdClan)
{
	return int32();
}

int32 USteamFriends::GetClanChatMessage(FSteamId SteamIdClan, int32 MessageIndex, const TArray<uint8>& Text, FSteamId& SteamIdUser, FString& ChatEntryType)
{
	return int32();
}

int32 USteamFriends::GetClanCount()
{
	return int32();
}

FString USteamFriends::GetClanName(FSteamId SteamIdClan)
{
	return FString();
}

FSteamId USteamFriends::GetClanOfficerByIndex(FSteamId SteamIdClan, int32 OfficerIndex)
{
	return FSteamId();
}

int32 USteamFriends::GetClanOfficerCount(FSteamId SteamIdClan)
{
	return int32();
}

FSteamId USteamFriends::GetClanOwner(FSteamId SteamIdClan)
{
	return FSteamId();
}

FString USteamFriends::GetClanTag(FSteamId SteamIdClan)
{
	return FString();
}

FSteamId USteamFriends::GetCoplayFriend(int32 CoplayFriend)
{
	return FSteamId();
}

int32 USteamFriends::GetCoplayFriendCount()
{
	return int32();
}

FSteamId USteamFriends::GetFriendByIndex(int32 FriendIndex, TArray<TEnumAsByte<ESteamFriendFlags>> FriendFlags)
{
	return FSteamId();
}

FSteamAppId USteamFriends::GetFriendCoplayGame(FSteamId SteamIdFriend)
{
	return FSteamAppId();
}

FDateTime USteamFriends::GetFriendCoplayTime(FSteamId SteamIdFriend)
{
	return FDateTime();
}

int32 USteamFriends::GetFriendCount(TArray<TEnumAsByte<ESteamFriendFlags>> FriendFlags)
{
	return int32();
}

int32 USteamFriends::GetFriendCountFromSource(FSteamId SteamIdSource)
{
	return int32();
}

FSteamId USteamFriends::GetFriendFromSourceByIndex(FSteamId SteamIdSource, int32 FriendIndex)
{
	return FSteamId();
}

bool USteamFriends::GetFriendGamePlayed(FSteamId SteamIdFriend, FSteamFriendGameInfo& GameInfo)
{
	return false;
}

int32 USteamFriends::GetFriendMessage(FSteamId SteamIdFriend, int32 MessageIndex, TArray<uint8>& Text, FString& ChatEntryType)
{
	return int32();
}

FString USteamFriends::GetFriendPersonaName(FSteamId SteamIdFriend)
{
	return FString();
}

FString USteamFriends::GetFriendPersonaNameHistory(FSteamId SteamIdFriend, int32 PersonaNameIndex)
{
	return FString();
}

TEnumAsByte<ESteamPersonaState> USteamFriends::GetFriendPersonaState(FSteamId SteamIdFriend)
{
	return TEnumAsByte<ESteamPersonaState>();
}

TEnumAsByte<ESteamFriendRelationship> USteamFriends::GetFriendRelationship(FSteamId SteamIdFriend)
{
	return TEnumAsByte<ESteamFriendRelationship>();
}

FString USteamFriends::GetFriendRichPresence(FSteamId SteamIdFriend, const FString& Key)
{
	return FString();
}

FString USteamFriends::GetFriendRichPresenceKeyByIndex(FSteamId SteamIdFriend, int32 KeyIndex)
{
	return FString();
}

int32 USteamFriends::GetFriendRichPresenceKeyCount(FSteamId SteamIdFriend)
{
	return int32();
}

int32 USteamFriends::GetFriendGroupCount()
{
	return int32();
}

FSteamFriendsGroupID USteamFriends::GetFriendsGroupIDByIndex(int32 GroupIndex)
{
	return FSteamFriendsGroupID();
}

int32 USteamFriends::GetFriendGroupMembersCount(FSteamFriendsGroupID FriendsGroupID)
{
	return int32();
}

TArray<FSteamId> USteamFriends::GetFriendsGroupMembersList(FSteamFriendsGroupID FriendsGroupID, int32 MembersCount)
{
	return TArray<FSteamId>();
}

FString USteamFriends::GetFriendsGroupName(FSteamFriendsGroupID FriendsGroupID)
{
	return FString();
}

int32 USteamFriends::GetFriendSteamLevel(FSteamId SteamIdFriend)
{
	return int32();
}

UTexture2D* USteamFriends::GetLargeFriendAvatar(FSteamId SteamIdFriend, int32& Avatar)
{
	return nullptr;
}

UTexture2D* USteamFriends::GetMediumFriendAvatar(FSteamId SteamIdFriend, int32& Avatar)
{
	return nullptr;
}

UTexture2D* USteamFriends::GetSmallFriendAvatar(FSteamId SteamIdFriend, int32& Avatar)
{
	return nullptr;
}

FString USteamFriends::GetPersonaName()
{
	return FString();
}

TEnumAsByte<ESteamPersonaState> USteamFriends::GetPersonaState()
{
	return TEnumAsByte<ESteamPersonaState>();
}

FString USteamFriends::GetPlayerNickname(FSteamId SteamIdPlayer)
{
	return FString();
}

ESteamUserRestriction USteamFriends::GetUserRestrictions()
{
	return ESteamUserRestriction();
}

bool USteamFriends::HasFriend(FSteamId SteamIdFriend, TArray<TEnumAsByte<ESteamFriendFlags>> FriendFlags)
{
	return false;
}

bool USteamFriends::InviteUserToGame(FSteamId SteamIdFriend, const FString& ConnectString)
{
	return false;
}

bool USteamFriends::IsClanChatAdmin(FSteamId SteamIdClanChat, FSteamId SteamIdUser)
{
	return false;
}

bool USteamFriends::IsClanPublic(FSteamId SteamIdClan)
{
	return false;
}

bool USteamFriends::IsClanOfficialGameGroup(FSteamId SteamIdClan)
{
	return false;
}

bool USteamFriends::IsClanChatWindowOpenInSteam(FSteamId SteamIdClanChat)
{
	return false;
}

bool USteamFriends::IsFollowing(FSteamId SteamIdFriend)
{
	return false;
}

bool USteamFriends::IsUserInSource(FSteamId SteamIdFriend, FSteamId SteamIdSource)
{
	return false;
}

bool USteamFriends::LeaveClanChatRoom(FSteamId SteamIdClanChat)
{
	return false;
}

bool USteamFriends::ReplyToFriendMessage(FSteamId SteamIdFriend, const FString& Text)
{
	return false;
}

void USteamFriends::RequestFriendRichPresence(FSteamId SteamIdFriend)
{
}

bool USteamFriends::RequestUserInfo(FSteamId SteamIdUser, bool bRequireNameOnly)
{
	return false;
}

bool USteamFriends::SendClanChatMessage(FSteamId SteamIdClanChat, const FString& Text)
{
	return false;
}

void USteamFriends::SetInGameVoiceSpeaking(FSteamId SteamIdFriend, bool bSpeaking)
{
}

void USteamFriends::SetListenForFriendMessage(bool bInterceptEnabled)
{
}

void USteamFriends::SetPlayedWith(FSteamId SteamIdUser)
{
}

bool USteamFriends::SetRichPresence(const FString& Key, const FString& Value)
{
	return false;
}

bool USteamFriends::BHasEquippedProfileItem(FSteamId SteamIdUser, TEnumAsByte<ESteamCommunityProfileItemType> ItemType)
{
	return false;
}

FString USteamFriends::GetProfileItemPropertyString(FSteamId SteamIdUser, TEnumAsByte<ESteamCommunityProfileItemType> ItemType, TEnumAsByte<ESteamCommunityProfileItemProperty> Property)
{
	return FString();
}

int32 USteamFriends::GetProfileItemPropertyUint(FSteamId SteamIdUser, TEnumAsByte<ESteamCommunityProfileItemType> ItemType, TEnumAsByte<ESteamCommunityProfileItemProperty> Property)
{
	return int32();
}
