// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/SessionEntry.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Subsystems/MultiplayerSessionSubsystem.h"
#include "Widgets/SessionEntryData.h"

USessionEntry::USessionEntry(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SessionSearchResultIndex = -1;
}

void USessionEntry::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);

	USessionEntryData* Data = Cast<USessionEntryData>(ListItemObject);

	if (IsValid(Data) == false)
	{
		return;
	}

	if (IsValid(Button_Join))
	{
		Button_Join->OnReleased.Clear();
		Button_Join->OnReleased.AddDynamic(this, &USessionEntry::JoinButtonReleased);
	}

	if (IsValid(TextBlock_SessionName))
	{
		TextBlock_SessionName->SetText(FText::FromString(Data->SessionName));
	}

	if (IsValid(TextBlock_MapName))
	{
		TextBlock_MapName->SetText(FText::FromString(Data->MapName));
	}

	if (IsValid(TextBlock_UserName))
	{
		FString Name = Data->UserName;
		if (Name.Len() > 16)
		{
			Name = Name.Left(16);
		}
		TextBlock_UserName->SetText(FText::FromString(Name));
	}

	if (IsValid(TextBlock_PlayersNum))
	{
		FString TempStr = FString::Printf(TEXT("%d/%d"), Data->CurrentPlayerNum, Data->MaxPlayerNum);
		TextBlock_PlayersNum->SetText(FText::FromString(TempStr));
	}

	if (IsValid(TextBlock_Ping))
	{
		FString TempStr = FString::Printf(TEXT("%dms"), Data->Ping);
		TextBlock_Ping->SetText(FText::FromString(TempStr));
	}

	SessionSearchResultIndex = Data->SessionSearchResultIndex;
}

void USessionEntry::JoinButtonReleased()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (IsValid(GameInstance))
	{
		UMultiplayerSessionSubsystem* Subsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();

		if (IsValid(Subsystem))
		{
			Subsystem->JoinSession(SessionSearchResultIndex);
		}
	}
}
