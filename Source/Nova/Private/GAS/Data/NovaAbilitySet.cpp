// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Data/NovaAbilitySet.h"

#include "GAS/NovaAbilitySystemComponent.h"

UNovaAbilitySet::UNovaAbilitySet(const FObjectInitializer& ObjectInitializer)
{
	
}

void UNovaAbilitySet::GiveToAbilitySystem(UNovaAbilitySystemComponent* NovaASC, TArray<struct FGameplayAbilitySpecHandle>& OutGrantedHandles, UObject* SourceObject) const
{
	check(NovaASC);

	if (NovaASC->IsOwnerActorAuthoritative() == false)
	{
		return;
	}

	for (const FNovaAbilitySetItem& AbilityToGrant : GrantedAbilities)
	{
		if (IsValid(AbilityToGrant.Ability) == false)
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(AbilityToGrant.Ability);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.Level = 1;
		
		if (AbilityToGrant.AbilityTag.IsValid())
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.AbilityTag);
		}

		if (AbilityToGrant.InputTag.IsValid())
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);
		}

		const FGameplayAbilitySpecHandle AbilitySpecHandle = NovaASC->GiveAbility(AbilitySpec);
		OutGrantedHandles.Add(AbilitySpecHandle);
	}
}
