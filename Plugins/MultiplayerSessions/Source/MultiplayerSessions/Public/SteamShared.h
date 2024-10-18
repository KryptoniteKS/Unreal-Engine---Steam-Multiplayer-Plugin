// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include <steam/isteammatchmaking.h>

#include "SteamShared.generated.h"

/* https://partner.steamgames.com/doc/api/ISteamMatchmaking#ELobbyType */
UENUM(BlueprintType)
enum ESteamLobbyType
{
	LobbyTypePrivate = 0 UMETA(DisplayName = "Private"),
	LobbyTypeFriendsOnly = 1 UMETA(DisplayName = "Friends Only"),
	LobbyTypePublic = 2 UMETA(DisplayName = "Public"),
	LobbyTypeInvisible = 3 UMETA(DisplayName = "Invisible"),
};

/* https://partner.steamgames.com/doc/api/steam_api#CSteamID */
USTRUCT(BlueprintType)
struct FSteamId
{
	GENERATED_BODY()

	UPROPERTY()
	int64 Result = 0;

	CSteamID GetSteamID() const
	{
		CSteamID SteamID;
		SteamID.SetFromUint64(Result);
		return SteamID;
	}
	FSteamId()
	{
		Result = 0;
	}
	FSteamId(int64 SteamID)
	{
		Result = SteamID;
	}

	FSteamId(CSteamID SteamID)
	{
		Result = SteamID.ConvertToUint64();
	}

	FSteamId(FString SteamID)
	{
		Result = FCString::Atoi64(*SteamID);
	}

	CSteamID GetSteamID()
	{
		CSteamID SteamID;
		SteamID.SetFromUint64(Result);
		return SteamID;
	}
};

UENUM(BlueprintType)
enum ESteamResult
{
	ResultNone = 0,							// no result
	ResultOK = 1,							// success
	ResultFail = 2,							// generic failure 
	ResultNoConnection = 3,					// no/failed network connection
	//	ResultNoConnectionRetry = 4,				// OBSOLETE - removed
	ResultInvalidPassword = 5,				// password/ticket is invalid
	ResultLoggedInElsewhere = 6,				// same user logged in elsewhere
	ResultInvalidProtocolVer = 7,			// protocol version is incorrect
	ResultInvalidParam = 8,					// a parameter is incorrect
	ResultFileNotFound = 9,					// file was not found
	ResultBusy = 10,							// called method busy - action not taken
	ResultInvalidState = 11,					// called object was in an invalid state
	ResultInvalidName = 12,					// name is invalid
	ResultInvalidEmail = 13,					// email is invalid
	ResultDuplicateName = 14,				// name is not unique
	ResultAccessDenied = 15,					// access is denied
	ResultTimeout = 16,						// operation timed out
	ResultBanned = 17,						// VAC2 banned
	ResultAccountNotFound = 18,				// account not found
	ResultInvalidSteamID = 19,				// steamID is invalid
	ResultServiceUnavailable = 20,			// The requested service is currently unavailable
	ResultNotLoggedOn = 21,					// The user is not logged on
	ResultPending = 22,						// Request is pending (may be in process, or waiting on third party)
	ResultEncryptionFailure = 23,			// Encryption or Decryption failed
	ResultInsufficientPrivilege = 24,		// Insufficient privilege
	ResultLimitExceeded = 25,				// Too much of a good thing
	ResultRevoked = 26,						// Access has been revoked (used for revoked guest passes)
	ResultExpired = 27,						// License/Guest pass the user is trying to access is expired
	ResultAlreadyRedeemed = 28,				// Guest pass has already been redeemed by account, cannot be acked again
	ResultDuplicateRequest = 29,				// The request is a duplicate and the action has already occurred in the past, ignored this time
	ResultAlreadyOwned = 30,					// All the games in this guest pass redemption request are already owned by the user
	ResultIPNotFound = 31,					// IP address not found
	ResultPersistFailed = 32,				// failed to write change to the data store
	ResultLockingFailed = 33,				// failed to acquire access lock for this operation
	ResultLogonSessionReplaced = 34,
	ResultConnectFailed = 35,
	ResultHandshakeFailed = 36,
	ResultIOFailure = 37,
	ResultRemoteDisconnect = 38,
	ResultShoppingCartNotFound = 39,			// failed to find the shopping cart requested
	ResultBlocked = 40,						// a user didn't allow it
	ResultIgnored = 41,						// target is ignoring sender
	ResultNoMatch = 42,						// nothing matching the request found
	ResultAccountDisabled = 43,
	ResultServiceReadOnly = 44,				// this service is not accepting content changes right now
	ResultAccountNotFeatured = 45,			// account doesn't have value, so this feature isn't available
	ResultAdministratorOK = 46,				// allowed to take this action, but only because requester is admin
	ResultContentVersion = 47,				// A Version mismatch in content transmitted within the Steam protocol.
	ResultTryAnotherCM = 48,					// The current CM can't service the user making a request, user should try another.
	ResultPasswordRequiredToKickSession = 49,// You are already logged in elsewhere, this cached credential login has failed.
	ResultAlreadyLoggedInElsewhere = 50,		// You are already logged in elsewhere, you must wait
	ResultSuspended = 51,					// Long running operation (content download) suspended/paused
	ResultCancelled = 52,					// Operation canceled (typically by user: content download)
	ResultDataCorruption = 53,				// Operation canceled because data is ill formed or unrecoverable
	ResultDiskFull = 54,						// Operation canceled - not enough disk space.
	ResultRemoteCallFailed = 55,				// an remote call or IPC call failed
	ResultPasswordUnset = 56,				// Password could not be verified as it's unset server side
	ResultExternalAccountUnlinked = 57,		// External account (PSN, Facebook...) is not linked to a Steam account
	ResultPSNTicketInvalid = 58,				// PSN ticket was invalid
	ResultExternalAccountAlreadyLinked = 59,	// External account (PSN, Facebook...) is already linked to some other account, must explicitly request to replace/delete the link first
	ResultRemoteFileConflict = 60,			// The sync cannot resume due to a conflict between the local and remote files
	ResultIllegalPassword = 61,				// The requested new password is not legal
	ResultSameAsPreviousValue = 62,			// new value is the same as the old one ( secret question and answer )
	ResultAccountLogonDenied = 63,			// account login denied due to 2nd factor authentication failure
	ResultCannotUseOldPassword = 64,			// The requested new password is not legal
	ResultInvalidLoginAuthCode = 65,			// account login denied due to auth code invalid
	ResultAccountLogonDeniedNoMail = 66,		// account login denied due to 2nd factor auth failure - and no mail has been sent
	ResultHardwareNotCapableOfIPT = 67,		// 
	ResultIPTInitError = 68,					// 
	ResultParentalControlRestricted = 69,	// operation failed due to parental control restrictions for current user
	ResultFacebookQueryError = 70,			// Facebook query returned an error
	ResultExpiredLoginAuthCode = 71,			// account login denied due to auth code expired
	ResultIPLoginRestrictionFailed = 72,
	ResultAccountLockedDown = 73,
	ResultAccountLogonDeniedVerifiedEmailRequired = 74,
	ResultNoMatchingURL = 75,
	ResultBadResponse = 76,					// parse failure, missing field, etc.
	ResultRequirePasswordReEntry = 77,		// The user cannot complete the action until they re-enter their password
	ResultValueOutOfRange = 78,				// the value entered is outside the acceptable range
	ResultUnexpectedError = 79,				// something happened that we didn't expect to ever happen
	ResultDisabled = 80,						// The requested service has been configured to be unavailable
	ResultInvalidCEGSubmission = 81,			// The set of files submitted to the CEG server are not valid !
	ResultRestrictedDevice = 82,				// The device being used is not allowed to perform this action
	ResultRegionLocked = 83,					// The action could not be complete because it is region restricted
	ResultRateLimitExceeded = 84,			// Temporary rate limit exceeded, try again later, different from ResultLimitExceeded which may be permanent
	ResultAccountLoginDeniedNeedTwoFactor = 85,	// Need two-factor code to login
	ResultItemDeleted = 86,					// The thing we're trying to access has been deleted
	ResultAccountLoginDeniedThrottle = 87,	// login attempt failed, try to throttle response to possible attacker
	ResultTwoFactorCodeMismatch = 88,		// two factor code mismatch
	ResultTwoFactorActivationCodeMismatch = 89,	// activation code for two-factor didn't match
	ResultAccountAssociatedToMultiplePartners = 90,	// account has been associated with multiple partners
	ResultNotModified = 91,					// data not modified
	ResultNoMobileDevice = 92,				// the account does not have a mobile device associated with it
	ResultTimeNotSynced = 93,				// the time presented is out of range or tolerance
	ResultSmsCodeFailed = 94,				// SMS code failure (no match, none pending, etc.)
	ResultAccountLimitExceeded = 95,			// Too many accounts access this resource
	ResultAccountActivityLimitExceeded = 96,	// Too many changes to this account
	ResultPhoneActivityLimitExceeded = 97,	// Too many changes to this phone
	ResultRefundToWallet = 98,				// Cannot refund to payment method, must use wallet
	ResultEmailSendFailure = 99,				// Cannot send an email
	ResultNotSettled = 100,					// Can't perform operation till payment has settled
	ResultNeedCaptcha = 101,					// Needs to provide a valid captcha
	ResultGSLTDenied = 102,					// a game server login token owned by this token's owner has been banned
	ResultGSOwnerDenied = 103,				// game server owner is denied for other reason (account lock, community ban, vac ban, missing phone)
	ResultInvalidItemType = 104,				// the type of thing we were requested to act on is invalid
	ResultIPBanned = 105,					// the ip address has been banned from taking this action
	ResultGSLTExpired = 106,					// this token has expired from disuse; can be reset for use
	ResultInsufficientFunds = 107,			// user doesn't have enough wallet funds to complete the action
	ResultTooManyPending = 108,				// There are too many of this thing pending already
	ResultNoSiteLicensesFound = 109,			// No site licenses found
	ResultWGNetworkSendExceeded = 110,		// the WG couldn't send a response because we exceeded max network send size
	ResultAccountNotFriends = 111,			// the user is not mutually friends
	ResultLimitedUserAccount = 112,			// the user is limited
	ResultCantRemoveItem = 113,				// item can't be removed
	ResultAccountDeleted = 114,				// account has been deleted
	ResultExistingUserCancelledLicense = 115,	// A license for this already exists, but cancelled
	ResultCommunityCooldown = 116,			// access is denied because of a community cooldown (probably from support profile data resets)
	ResultNoLauncherSpecified = 117,			// No launcher was specified, but a launcher was needed to choose correct realm for operation.
	ResultMustAgreeToSSA = 118,				// User must agree to china SSA or global SSA before login
	ResultLauncherMigrated = 119,			// The specified launcher type is no longer supported; the user should be directed elsewhere
	ResultSteamRealmMismatch = 120,			// The user's realm does not match the realm of the requested resource
	ResultInvalidSignature = 121,			// signature check did not match
	ResultParseFailure = 122,				// Failed to parse input
	ResultNoVerifiedPhone = 123,				// account does not have a verified phone number
};


UCLASS()
class MULTIPLAYERSESSIONS_API USteamShared : public UObject
{
	GENERATED_BODY()
	
};
