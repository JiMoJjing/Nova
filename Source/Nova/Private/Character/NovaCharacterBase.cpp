// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/NovaCharacterBase.h"

#include "NovaGameplayTags.h"
#include "Engine/LocalPlayer.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"

ANovaCharacterBase::ANovaCharacterBase()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	GetMesh()->SetReceivesDecals(false);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	bIsSelected = false;
	
	FactionTag = NovaGameplayTags::Faction_Enemy;
}

void ANovaCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	OnFactionTagChanged();
}

void ANovaCharacterBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ANovaCharacterBase::OnFactionTagChanged()
{
	if (FactionTag == NovaGameplayTags::Faction_Enemy)
	{
		OutlineStencilValue = 255;
	}
	else if (FactionTag == NovaGameplayTags::Faction_Player)
	{
		OutlineStencilValue = 254;
	}
	else if (FactionTag == NovaGameplayTags::Faction_NPC)
	{
		OutlineStencilValue = 253;
	}
	
	GetMesh()->SetCustomDepthStencilValue(OutlineStencilValue);
}

void ANovaCharacterBase::OnHovered()
{
	if (bIsSelected == true)
	{
		return;
	}
	
	GetMesh()->SetRenderCustomDepth(true);
}

void ANovaCharacterBase::OnUnhovered()
{
	if (bIsSelected == true)
	{
		return;
	}
	
	GetMesh()->SetRenderCustomDepth(false);
}

void ANovaCharacterBase::OnSelected()
{
	bIsSelected = true;
	GetMesh()->SetRenderCustomDepth(false);
}

void ANovaCharacterBase::OnDeselected()
{
	bIsSelected = false;
	GetMesh()->SetRenderCustomDepth(false);
}

void ANovaCharacterBase::GetTargetBounds(float& OutHalfWidth, float& OutHalfHeight) const
{
	GetCapsuleComponent()->GetScaledCapsuleSize(OutHalfWidth, OutHalfHeight);
}

FGameplayTag ANovaCharacterBase::GetFactionTag() const
{
	return FactionTag;
}

void ANovaCharacterBase::SetFactionTag(const FGameplayTag& NewFactionTag)
{
	FactionTag = NewFactionTag;
}
