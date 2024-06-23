// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionEntry.h"
#include "SessionEntryDataWrapper.h"

void USessionEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	USessionEntryDataWrapper* DataWrapper = Cast<USessionEntryDataWrapper>(ListItemObject);
	if (DataWrapper)
	{
		SessionDetails = DataWrapper->SessionInfo;
	}
}
