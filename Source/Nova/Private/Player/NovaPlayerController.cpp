// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NovaPlayerController.h"

#include "GAS/NovaAbilitySystemComponent.h"
#include "Player/NovaPlayerState.h"

ANovaPlayerController::ANovaPlayerController(const FObjectInitializer& ObjectInitializer)
{
}

ANovaPlayerState* ANovaPlayerController::GetNovaPlayerState() const
{
	return CastChecked<ANovaPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UNovaAbilitySystemComponent* ANovaPlayerController::GetNovaAbilitySystemComponent() const
{
	const ANovaPlayerState* NovaPS = GetNovaPlayerState();
	if (NovaPS)
	{
		return NovaPS->GetNovaAbilitySystemComponent();
	}

	return nullptr;
}

void ANovaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
}

void ANovaPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ANovaPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UNovaAbilitySystemComponent* NovaASC = GetNovaAbilitySystemComponent())
	{
		NovaASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}
