// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "NovaAbilitySystemComponent.generated.h"

/**
 *	Author: 지용현
 *	Date: 2026.01.06(2026.01.06)
 *	ASC에서 Tag기반 Ability입력 처리 기능 추가
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOVA_API UNovaAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	UNovaAbilitySystemComponent(const FObjectInitializer& ObjectInitializer);

	void AbilityInputTagPressed(FGameplayTag& InputTag);
	void AbilityInputTagReleased(FGameplayTag& InputTag);

	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();

protected:
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;

protected:
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
};