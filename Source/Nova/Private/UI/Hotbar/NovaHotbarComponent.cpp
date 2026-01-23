// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Hotbar/NovaHotbarComponent.h"
#include "GAS/NovaAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Player/NovaPlayerController.h"

UNovaHotbarComponent::UNovaHotbarComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(false);
}

void UNovaHotbarComponent::BeginPlay()
{
	Super::BeginPlay();

	if (Slots.Num() == 0)
	{
		Slots.SetNum(DefaultSlotCount);
	}
}

void UNovaHotbarComponent::SetSlotAbility(int32 SlotIndex, FGameplayTag AbilityTag)
{
	if (Slots.IsValidIndex(SlotIndex))
	{
		if (Slots[SlotIndex].AbilityTag != AbilityTag)
		{
			Slots[SlotIndex].AbilityTag = AbilityTag;

			OnSlotsChanged.Broadcast();
		}
	}
}

void UNovaHotbarComponent::ClearSlot(int32 SlotIndex)
{
	SetSlotAbility(SlotIndex, FGameplayTag());
}

void UNovaHotbarComponent::UseSlot(int32 SlotIndex)
{
	if (Slots.IsValidIndex(SlotIndex))
	{
		FGameplayTag& TagToActivate = Slots[SlotIndex].AbilityTag;
		
		if (TagToActivate.IsValid())
		{
			ANovaPlayerController* NovaPC = Cast<ANovaPlayerController>(GetOwner());
			if (NovaPC != nullptr)
			{
				UNovaAbilitySystemComponent* NovaASC = NovaPC->GetNovaAbilitySystemComponent();
				if (NovaASC != nullptr)
				{
					NovaASC->AbilityInputTagPressed(TagToActivate);
				}
			}
		}
	}
}