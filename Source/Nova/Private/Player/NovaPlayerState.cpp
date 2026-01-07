// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NovaPlayerState.h"

#include "GAS/NovaAbilitySystemComponent.h"
#include "Player/NovaPlayerController.h"

ANovaPlayerState::ANovaPlayerState(const FObjectInitializer& ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UNovaAbilitySystemComponent>(TEXT("ASC"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	NetUpdateFrequency = 100.0f;	
}

ANovaPlayerController* ANovaPlayerState::GetNovaPlayerController() const
{
	return Cast<ANovaPlayerController>(GetOwner());
}

UAbilitySystemComponent* ANovaPlayerState::GetAbilitySystemComponent() const
{
	return GetNovaAbilitySystemComponent();
}
