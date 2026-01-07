// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/NovaInputConfig.h"

UNovaInputConfig::UNovaInputConfig(const FObjectInitializer& ObjectInitializer)
{
}

const UInputAction* UNovaInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FNovaInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	return nullptr;
}

const UInputAction* UNovaInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag) const
{
	for (const FNovaInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	return nullptr;
}
