// Copyright Epic Games, Inc. All Rights Reserved.

#include "Character/NovaCharacter.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Character/Components/NovaHeroComponent.h"
#include "Character/Components/NovaPawnExtensionComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetSystemLibrary.h"

ANovaCharacter::ANovaCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	GetMesh()->SetReceivesDecals(false);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;


	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bDoCollisionTest = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	NovaPawnExtensionComponent = CreateDefaultSubobject<UNovaPawnExtensionComponent>(TEXT("PawnExtension"));
	NovaHeroComponent = CreateDefaultSubobject<UNovaHeroComponent>(TEXT("NovaHeroComponent"));

	bIsSelected = false;
}

void ANovaCharacter::BeginPlay()
{
	Super::BeginPlay();

	NovaPawnExtensionComponent->CheckDefaultInitialization();
	GetMesh()->SetCustomDepthStencilValue(OutlineStencilValue);
}

void ANovaCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ANovaCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	NovaPawnExtensionComponent->HandleControllerChanged();
}

void ANovaCharacter::UnPossessed()
{
	Super::UnPossessed();

	// @Todo: PawnExt->빙의 해제 처리
}

void ANovaCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();

	NovaPawnExtensionComponent->HandleControllerChanged();
}

void ANovaCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	NovaPawnExtensionComponent->HandlePlayerStateReplicated();
}

void ANovaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	NovaPawnExtensionComponent->SetupPlayerInputComponent();
}

void ANovaCharacter::OnHovered()
{
	if (bIsSelected == true)
	{
		return;
	}
	
	GetMesh()->SetRenderCustomDepth(true);
}

void ANovaCharacter::OnUnhovered()
{
	if (bIsSelected == true)
	{
		return;
	}
	
	GetMesh()->SetRenderCustomDepth(false);
}

void ANovaCharacter::OnSelected()
{
	bIsSelected = true;
	GetMesh()->SetRenderCustomDepth(true);
}

void ANovaCharacter::OnDeselected()
{
	bIsSelected = false;
	GetMesh()->SetRenderCustomDepth(false);
}

void ANovaCharacter::GetTargetBounds(float& OutHalfWidth, float& OutHalfHeight) const
{
	GetCapsuleComponent()->GetScaledCapsuleSize(OutHalfWidth, OutHalfHeight);
}
