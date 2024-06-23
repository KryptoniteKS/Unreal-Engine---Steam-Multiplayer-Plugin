// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionMenu.h"
#include "SessionEntryDataWrapper.h"

void USessionMenu::AddItemToList(UObject* SessionItem)
{
	USessionEntryDataWrapper* Item = Cast<USessionEntryDataWrapper>(SessionItem);
	if (Item)
	{
		List->AddItem(Item);
	}
}
