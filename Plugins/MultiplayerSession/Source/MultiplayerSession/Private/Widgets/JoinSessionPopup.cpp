// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/JoinSessionPopup.h"

#include "OnlineSessionSettings.h"
#include "Animation/WidgetAnimation.h"
#include "Subsystems/MultiplayerSessionSubsystem.h"
#include "Widgets/SessionMenu.h"
#include "Components/Button.h"
#include "Components/EditableText.h"
#include "Online/OnlineSessionNames.h"
#include "Widgets/SessionEntryData.h"
#include "Components/ListView.h"

UJoinSessionPopup::UJoinSessionPopup(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PopupSize = FVector2D(1280.0f, 780.0f);
}

void UJoinSessionPopup::NativeConstruct()
{
	Super::NativeConstruct();

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance != nullptr)
	{
		MultiplayerSessionSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
	}

	if (MultiplayerSessionSubsystem != nullptr)
	{
		MultiplayerSessionSubsystem->OnMultiplaySessionSearchCompleted.AddUObject(this, &UJoinSessionPopup::OnFindSession);
	}

	if (IsValid(Button_Back))
	{
		Button_Back->OnReleased.AddDynamic(this, &UJoinSessionPopup::OnBackButtonReleased);
	}

	if (IsValid(Button_Search))
	{
		Button_Search->OnReleased.AddDynamic(this, &UJoinSessionPopup::OnSearchButtonReleased);
	}

	if (IsValid(Button_Refresh))
	{
		Button_Refresh->OnReleased.AddDynamic(this, &UJoinSessionPopup::OnRefreshButtonReleased);
	}

	if (IsValid(FloatingAnimation))
	{
		FWidgetAnimationDynamicEvent AnimationFinishedDelegate;
		AnimationFinishedDelegate.BindDynamic(this, &UJoinSessionPopup::OnFloatingAnimationFinished);
		BindToAnimationFinished(FloatingAnimation, AnimationFinishedDelegate);
		PlayAnimation(FloatingAnimation, 0.0f, 1);
	}
}

void UJoinSessionPopup::OnFindSession(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful)
{
	if (IsValid(MultiplayerSessionSubsystem) == false)
	{
		return;
	}

	if (IsValid(ListView_Sessions) == false)
	{
		return;
	}

	if (bWasSuccessful == false)
	{
		return;
	}
	
	ListView_Sessions->ClearListItems();

	if (SearchResults.Num() <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, TEXT("No Search Results."));
		// for (int32 i = 1; i <= 20; ++i)
		// {
		// 	FString SessionName = FString::Printf(TEXT("TestSession%d"),i);
		// 	FString MapName = FString::Printf(TEXT("Map%d"),i);
		// 	FString UserName = FString::Printf(TEXT("Test ID%d"), i);
		// 	int32 MaxPlayerNum = 8;
		// 	int32 CurrentPlayerNum = MaxPlayerNum - 7;
		// 	int32 Ping = i * 2;
		//
		// 	USessionEntryData* NewEntryData = NewObject<USessionEntryData>();
		// 	NewEntryData->SessionName = SessionName;
		// 	NewEntryData->MapName = MapName;
		// 	NewEntryData->UserName = UserName;
		// 	NewEntryData->MaxPlayerNum = MaxPlayerNum;
		// 	NewEntryData->CurrentPlayerNum = CurrentPlayerNum;
		// 	NewEntryData->Ping = Ping;
		// 	NewEntryData->SessionSearchResultIndex = i;
		// 	
		// 	if (IsValid(ListView_Sessions))
		// 	{
		// 		ListView_Sessions->AddItem(NewEntryData);
		// 	}
		// }
		return;
	}

	FName SessionNameKey = MultiplayerSessionSubsystem->SESSION_NAME_KEY;
	FName MapNameKey = MultiplayerSessionSubsystem->MAP_NAME_KEY;

	for (int32 i = 0; i < SearchResults.Num(); ++i)
	{
		FOnlineSessionSearchResult SearchResult = SearchResults[i];
		
		FString SessionName;
		SearchResult.Session.SessionSettings.Get(SessionNameKey, SessionName);

		FString MapName;
		SearchResult.Session.SessionSettings.Get(MapNameKey, MapName);

		FString UserName = SearchResult.Session.OwningUserName;
		int32 MaxPlayerNum = SearchResult.Session.SessionSettings.NumPublicConnections;
		int32 CurrentPlayerNum = MaxPlayerNum - SearchResult.Session.NumOpenPublicConnections;
		int32 Ping = SearchResult.PingInMs;

		USessionEntryData* NewEntryData = NewObject<USessionEntryData>();
		NewEntryData->SessionName = SessionName;
		NewEntryData->MapName = MapName;
		NewEntryData->UserName = UserName;
		NewEntryData->MaxPlayerNum = MaxPlayerNum;
		NewEntryData->CurrentPlayerNum = CurrentPlayerNum;
		NewEntryData->Ping = Ping;
		NewEntryData->SessionSearchResultIndex = i;

		if (IsValid(ListView_Sessions))
		{
			ListView_Sessions->AddItem(NewEntryData);
		}
	}
}

void UJoinSessionPopup::OnBackButtonReleased()
{
	MultiplayerSessionSubsystem->OnMultiplaySessionSearchCompleted.RemoveAll(this);
	OnRemoveFromParent.Broadcast();
	RemoveFromParent();
}

void UJoinSessionPopup::OnSearchButtonReleased()
{
	if (IsValid(MultiplayerSessionSubsystem) == false)
	{
		return;
	}
	
	ListView_Sessions->ClearListItems();
	const TArray<FOnlineSessionSearchResult> SearchResults = MultiplayerSessionSubsystem->GetSessionSearchResults();

	if (SearchResults.Num() <= 0)
	{
		return;
	}

	FString SearchKeyWords = TEXT("");
	if (IsValid(EditableText_SessionName))
	{
		SearchKeyWords = EditableText_SessionName->GetText().ToString();
	}

	FName SessionNameKey = MultiplayerSessionSubsystem->SESSION_NAME_KEY;
	FName MapNameKey = MultiplayerSessionSubsystem->MAP_NAME_KEY;
	
	for (int32 i = 0; i < SearchResults.Num(); ++i)
	{
		FOnlineSessionSearchResult SearchResult = SearchResults[i];
		FString SessionName;
		SearchResult.Session.SessionSettings.Get(SessionNameKey, SessionName);

		if (SessionName.Contains(SearchKeyWords) == false)
		{
			continue;
		}

		FString MapName;
		SearchResult.Session.SessionSettings.Get(MapNameKey, MapName);

		FString UserName = SearchResult.Session.OwningUserName;
		int32 MaxPlayerNum = SearchResult.Session.SessionSettings.NumPublicConnections;
		int32 CurrentPlayerNum = MaxPlayerNum - SearchResult.Session.NumOpenPublicConnections;
		int32 Ping = SearchResult.PingInMs;

		USessionEntryData* NewEntryData = NewObject<USessionEntryData>();
		NewEntryData->SessionName = SessionName;
		NewEntryData->MapName = MapName;
		NewEntryData->UserName = UserName;
		NewEntryData->MaxPlayerNum = MaxPlayerNum;
		NewEntryData->CurrentPlayerNum = CurrentPlayerNum;
		NewEntryData->Ping = Ping;
		NewEntryData->SessionSearchResultIndex = i;

		if (IsValid(ListView_Sessions))
		{
			ListView_Sessions->AddItem(NewEntryData);
		}
	}
}

void UJoinSessionPopup::OnRefreshButtonReleased()
{
	EditableText_SessionName->SetText(FText::FromString(TEXT("")));
	MultiplayerSessionSubsystem->FindSessions();
}

void UJoinSessionPopup::OnFloatingAnimationFinished()
{
	MultiplayerSessionSubsystem->FindSessions();
}
