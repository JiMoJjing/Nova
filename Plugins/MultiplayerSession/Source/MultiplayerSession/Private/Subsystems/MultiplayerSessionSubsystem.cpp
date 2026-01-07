// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSession/Public/Subsystems/MultiplayerSessionSubsystem.h"

#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemTypes.h"
#include "Online/OnlineSessionNames.h"

const FName UMultiplayerSessionSubsystem::SESSION_NAME_KEY = FName(TEXT("SERVERNAME"));
const FName UMultiplayerSessionSubsystem::MAP_NAME_KEY = FName(TEXT("MAPNAME"));

void UMultiplayerSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		SessionInterface = OnlineSubsystem->GetSessionInterface();
	}
}

void UMultiplayerSessionSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UMultiplayerSessionSubsystem::CreateSession(FString InSessionName, FString InMapName, int32 InNumPublicConnections)
{
	if (SessionInterface.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface is not valid!"));
		return;
	}

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(NAME_GameSession);
	if (ExistingSession != nullptr)
	{
		RecreateSessionName = InSessionName;
		RecreateManName = InMapName;
		RecreateNumPublicConnections = InNumPublicConnections;
		
		OnDestroySessionCompletedDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(FOnDestroySessionCompleteDelegate::CreateUObject(this, &UMultiplayerSessionSubsystem::OnDestroySessionComplete));
		SessionInterface->DestroySession(NAME_GameSession);
		return;
	}

	const ULocalPlayer* LocalPlayer = GetGameInstance()->GetFirstGamePlayer();
	if (IsValid(LocalPlayer) == false || LocalPlayer->GetPreferredUniqueNetId().IsValid() == false)
	{
		return;
	}

	// Create Delegate 등록.
	OnCreateSessionCompletedDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionCompleteDelegate::CreateUObject(this, &UMultiplayerSessionSubsystem::OnCreateSessionComplete));
	
	// 세션 세팅.
	LastSessionSettings = MakeShared<FOnlineSessionSettings>();

	LastSessionSettings->bIsDedicated = false;									// Dedicated 아님 으로 설정.
	LastSessionSettings->bIsLANMatch = false;									// LAN모드 설정.
	LastSessionSettings->bAllowJoinInProgress = true;							// 나중에 조인 여부.
	LastSessionSettings->bAllowJoinViaPresence = true;							// 나중에 Presence로 조인?.
	LastSessionSettings->NumPublicConnections = InNumPublicConnections;			// 최대 인원 수.
	LastSessionSettings->bShouldAdvertise = true;								// 검색 가능 여부.
	LastSessionSettings->bUsesPresence = true;									// Steam Presence, Lobby 사용.
	LastSessionSettings->bUseLobbiesIfAvailable = true;							// Steam용 로비 사용 허용.
	LastSessionSettings->bAllowInvites = true;									// Steam 친구 초대 여부.
	LastSessionSettings->BuildUniqueId = 1;										// 서로 다른 빌드간의 매칭 구분
	
	// 여기서 세팅하는 이름이 실제로 노출되는 이름.
	// KEY는 정해져있는 게 아니라 내가 만드는 것.
	LastSessionSettings->Set(SESSION_NAME_KEY, InSessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	LastSessionSettings->Set(MAP_NAME_KEY, InMapName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// CreateSession의 SessionName은 해당 클라이언트/서버에서 세션을 구분하기 위한 내부 키.
	// 그러므로 NAME_GameSession이라는 상수를 사용한다.
	const bool bCreateSessionSuccess = SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings);
	if (bCreateSessionSuccess == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateSessionFailed"));
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompletedDelegateHandle);
		OnCreateSessionCompletedDelegateHandle.Reset();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CreateSessionSuccess!"));		
	}
}

void UMultiplayerSessionSubsystem::FindSessions()
{
	if (SessionInterface.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface is not valid!"));
		return;
	}

	OnFindSessionCompletedDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsCompleteDelegate::CreateUObject(this, &UMultiplayerSessionSubsystem::OnFindSessionComplete));

	LastSessionSearch = MakeShared<FOnlineSessionSearch>();
	LastSessionSearch->MaxSearchResults = 1000;
	LastSessionSearch->bIsLanQuery = false;
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	const bool bFindSessionSuccess = SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef());
	if (bFindSessionSuccess == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("FindSessionFailed"));
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionCompletedDelegateHandle);
		OnFindSessionCompletedDelegateHandle.Reset();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("FindSessionSuccess"));
	}
}

void UMultiplayerSessionSubsystem::JoinSession(int32 SessionSearchResultIndex)
{
	if (LastSessionSearch.IsValid() == false)
	{
		return;
	}

	if (LastSessionSearch->SearchResults.IsValidIndex(SessionSearchResultIndex) == false)
	{
		return;
	}

	OnJoinSessionCompletedDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionCompleteDelegate::CreateUObject(this, &UMultiplayerSessionSubsystem::OnJoinSessionComplete));
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, LastSessionSearch->SearchResults[SessionSearchResultIndex]);
}

void UMultiplayerSessionSubsystem::StartSession()
{
	if (SessionInterface.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface is not valid!"));
		return;
	}

	OnStartSessionCompletedDelegateHandle = SessionInterface->AddOnStartSessionCompleteDelegate_Handle(FOnStartSessionCompleteDelegate::CreateUObject(this, &UMultiplayerSessionSubsystem::OnStartSessionComplete));

	const bool bStartSessionSuccess = SessionInterface->StartSession(NAME_GameSession);
	if (bStartSessionSuccess == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("StartSessionFailed"));
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompletedDelegateHandle);
		OnStartSessionCompletedDelegateHandle.Reset();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("StartSessionSuccess"));
	}
}

void UMultiplayerSessionSubsystem::DestroySession()
{
	
}

const TArray<FOnlineSessionSearchResult>& UMultiplayerSessionSubsystem::GetSessionSearchResults() const
{
	return LastSessionSearch->SearchResults;
}

void UMultiplayerSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface is not valid!"));
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("OnCreateSessionComplete: %s, Success: %s"), *SessionName.ToString(), bWasSuccessful ? TEXT("true") : TEXT("false"));
	SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompletedDelegateHandle);
	OnCreateSessionCompletedDelegateHandle.Reset();

	if (bWasSuccessful == true)
	{
		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			World->ServerTravel(FString("/Game/ThirdPerson/Maps/Lobby?listen"));
		}
	}
}

void UMultiplayerSessionSubsystem::OnFindSessionComplete(bool bWasSuccessful)
{
	if (SessionInterface.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface is not valid!"));
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("OnFindSessionComplete / Success: %s"), bWasSuccessful ? TEXT("true") : TEXT("false"));	
	SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionCompletedDelegateHandle);
	OnFindSessionCompletedDelegateHandle.Reset();
	
	OnMultiplaySessionSearchCompleted.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);
}

void UMultiplayerSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface is not valid!"));
		return;
	}

	SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompletedDelegateHandle);
	OnJoinSessionCompletedDelegateHandle.Reset();

	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		return;
	}
	
	FString Address;
	const bool bWasSuccessful = SessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Cyan,
			FString::Printf(TEXT("Connect string: %s"), *Address)
		);
	}
	
	if (bWasSuccessful == true)
	{
		APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
		if (PlayerController != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete ClientTravel!!"));
			PlayerController->ClientTravel(Address, TRAVEL_Absolute);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("OnJoinSessionComplete Failed!!"));
	}
}

void UMultiplayerSessionSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
	if (SessionInterface.IsValid() == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("SessionInterface is not valid!"));
		return;
	}
	SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompletedDelegateHandle);
	OnStartSessionCompletedDelegateHandle.Reset();

	if (bWasSuccessful == true)
	{
		// @Todo_JYH: SeamlessTravel 하기.(GameMode)
	}
}

void UMultiplayerSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompletedDelegateHandle);
	OnDestroySessionCompletedDelegateHandle.Reset();

	CreateSession(RecreateSessionName, RecreateManName, RecreateNumPublicConnections);
}
