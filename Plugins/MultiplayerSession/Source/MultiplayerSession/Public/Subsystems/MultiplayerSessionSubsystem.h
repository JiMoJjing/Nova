// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionSubsystem.generated.h"


DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMultiplaySessionSearchCompleted, const TArray<FOnlineSessionSearchResult>& /* SessionSearchResults */, bool /* bWasSuccessful */);

/**
 * 
 */
UCLASS(BlueprintType)
class MULTIPLAYERSESSION_API UMultiplayerSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void CreateSession(FString InSessionName, FString InMapName, int32 InNumPublicConnections);

	UFUNCTION(BlueprintCallable)
	void FindSessions();

	UFUNCTION(BlueprintCallable)
	void JoinSession(int32 SessionSearchResultIndex);

	UFUNCTION(BlueprintCallable)
	void StartSession();

	UFUNCTION(BlueprintCallable)
	void DestroySession();

	const TArray<FOnlineSessionSearchResult>& GetSessionSearchResults() const;

private:
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

public:
	FOnMultiplaySessionSearchCompleted OnMultiplaySessionSearchCompleted;
	
private:
	IOnlineSessionPtr SessionInterface;

	TSharedPtr<FOnlineSessionSettings> LastSessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	FDelegateHandle OnCreateSessionCompletedDelegateHandle;
	FDelegateHandle OnFindSessionCompletedDelegateHandle;
	FDelegateHandle OnJoinSessionCompletedDelegateHandle;
	FDelegateHandle OnStartSessionCompletedDelegateHandle;
	FDelegateHandle OnDestroySessionCompletedDelegateHandle;

	FString RecreateSessionName;
	FString RecreateManName;
	int32 RecreateNumPublicConnections;
public:
	static const FName SESSION_NAME_KEY;
	static const FName MAP_NAME_KEY;
};