// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "NovaPlayerState.generated.h"

class UNovaAbilitySystemComponent;
class ANovaPlayerController;
/**
 *	Author: 지용현
 *	Date: 2026.01.06(2026.01.06)
 *	NovaASC, AttributeSet 관리
 */
UCLASS()
class NOVA_API ANovaPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ANovaPlayerState(const FObjectInitializer& ObjectInitializer);

	ANovaPlayerController* GetNovaPlayerController() const;

	UNovaAbilitySystemComponent* GetNovaAbilitySystemComponent() const { return AbilitySystemComponent; }
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY()
	TObjectPtr<UNovaAbilitySystemComponent> AbilitySystemComponent;

	
};