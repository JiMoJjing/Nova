// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSession/Public/GameModes/MultiplayerLobbyGameMode.h"

#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerState.h"

AMultiplayerLobbyGameMode::AMultiplayerLobbyGameMode()
{
}

void AMultiplayerLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (IsValid(GameState))
	{
		int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("Players in game: %d"), NumberOfPlayers));

			APlayerState* NewPlayerState = NewPlayer->GetPlayerState<APlayerState>();
			if (NewPlayerState)
			{
				FString NewPlayerName = NewPlayerState->GetPlayerName();
				GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Yellow, FString::Printf(TEXT("%s has joined the game!"), *NewPlayerName));
			}			
		}
	}
}

void AMultiplayerLobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	APlayerState* ExitingPlayerState = Exiting->GetPlayerState<APlayerState>();
	if (ExitingPlayerState)
	{
		FString ExitingPlayerName = ExitingPlayerState->GetPlayerName();
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("%s has exited the game!"), *ExitingPlayerName));
	}
}
