// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/NovaPawnExtensionComponent.h"

#include "AbilitySystemInterface.h"
#include "Data/NovaPawnData.h"
#include "GameFramework/PlayerState.h"
#include "GAS/NovaAbilitySystemComponent.h"
#include "GAS/Data/NovaAbilitySet.h"
#include "Input/NovaInputComponent.h"
#include "Player/NovaPlayerController.h"
#include "Player/NovaPlayerState.h"

UNovaPawnExtensionComponent::UNovaPawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

UNovaPawnExtensionComponent* UNovaPawnExtensionComponent::FindNovaPawnExtensionComponent(const AActor* Actor)
{
	if (Actor != nullptr)
	{
		return Actor->GetComponentByClass<UNovaPawnExtensionComponent>();
	}

	return nullptr;
}

void UNovaPawnExtensionComponent::CheckDefaultInitialization()
{
	if (bIsDefaultInitialized == true)
	{
		return;
	}

	APawn* Pawn = Cast<APawn>(GetOwner());
	if (Pawn == nullptr)
	{
		return;
	}

	ANovaPlayerController* NovaPC = Cast<ANovaPlayerController>(Pawn->GetController());
	if (NovaPC == nullptr)
	{
		return;
	}
	
	ANovaPlayerState* NovaPS = Cast<ANovaPlayerState>(Pawn->GetPlayerState());
	if (NovaPS == nullptr)
	{
		return;
	}

	UNovaInputComponent* NovaIC = Cast<UNovaInputComponent>(Pawn->InputComponent);
	if (NovaIC == nullptr)
	{
		return;
	}

	IAbilitySystemInterface* AbilitySystemInterface = Cast<IAbilitySystemInterface>(NovaPS);
	if (AbilitySystemInterface != nullptr)
	{
		UNovaAbilitySystemComponent* NovaASC = Cast<UNovaAbilitySystemComponent>(AbilitySystemInterface->GetAbilitySystemComponent());
		if (NovaASC != nullptr)
		{
			InitializeAbilitySystem(NovaASC, NovaPS);
		}
	}

	bIsDefaultInitialized = true;
	OnPawnReadyToInitialize.Broadcast();
}

void UNovaPawnExtensionComponent::InitializeAbilitySystem(UNovaAbilitySystemComponent* InASC, AActor* InOwnerActor)
{
	check(InASC);
	check(InOwnerActor);

	if (AbilitySystemComponent == InASC)
	{
		return;
	}

	if (AbilitySystemComponent != nullptr)
	{
		UninitializeAbilitySystem();
	}

	APawn* Pawn = Cast<APawn>(InOwnerActor);
	if (Pawn == nullptr)
	{
		return;
	}

	AActor* ExistingAvatar = InASC->GetAvatarActor();

	if ((ExistingAvatar != nullptr) && (ExistingAvatar != Pawn))
	{
		ensure(!ExistingAvatar->HasAuthority());

		if (UNovaPawnExtensionComponent* OtherExtensionComp = FindNovaPawnExtensionComponent(ExistingAvatar))
		{
			OtherExtensionComp->UninitializeAbilitySystem();
		}
	}
	AbilitySystemComponent = InASC;
	AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, Pawn);
	
	if (PawnData != nullptr)
	{
		TArray<FGameplayAbilitySpecHandle> GrantedHandles;
		PawnData->AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, GrantedHandles);
	}
	
	OnAbilitySystemInitialized.Broadcast();
}

void UNovaPawnExtensionComponent::UninitializeAbilitySystem()
{
	if (AbilitySystemComponent == nullptr)
	{
		return;
	}

	if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
	{
		AbilitySystemComponent->CancelAbilities();
		AbilitySystemComponent->ClearAbilityInput();
		AbilitySystemComponent->RemoveAllGameplayCues();

		if (AbilitySystemComponent->GetOwnerActor() != nullptr)
		{
			AbilitySystemComponent->SetAvatarActor(nullptr);
		}
		else
		{
			AbilitySystemComponent->ClearActorInfo();
		}

		OnAbilitySystemUninitialized.Broadcast();
	}

	AbilitySystemComponent = nullptr;
}

void UNovaPawnExtensionComponent::HandleControllerChanged()
{
	CheckDefaultInitialization();
}

void UNovaPawnExtensionComponent::HandlePlayerStateReplicated()
{
	CheckDefaultInitialization();
}

void UNovaPawnExtensionComponent::SetupPlayerInputComponent()
{
	CheckDefaultInitialization();
}