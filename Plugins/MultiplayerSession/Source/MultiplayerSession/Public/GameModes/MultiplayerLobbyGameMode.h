// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerLobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSION_API AMultiplayerLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMultiplayerLobbyGameMode();

	virtual void PostLogin(APlayerController* NewPlayer);
	virtual void Logout(AController* Exiting) override;
		
};
