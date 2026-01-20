// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/NovaHeroCharacter.h"

#include "NovaGameplayTags.h"
#include "Camera/CameraComponent.h"
#include "Character/Components/NovaHeroComponent.h"
#include "Character/Components/NovaPawnExtensionComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


ANovaHeroCharacter::ANovaHeroCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
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
	
	FactionTag = NovaGameplayTags::Faction_Player;
}

void ANovaHeroCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	NovaPawnExtensionComponent->CheckDefaultInitialization();
}

void ANovaHeroCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ANovaHeroCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	NovaPawnExtensionComponent->HandleControllerChanged();
}

void ANovaHeroCharacter::UnPossessed()
{
	Super::UnPossessed();
}

void ANovaHeroCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	
	NovaPawnExtensionComponent->HandleControllerChanged();
}

void ANovaHeroCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	NovaPawnExtensionComponent->HandlePlayerStateReplicated();
}

void ANovaHeroCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	NovaPawnExtensionComponent->SetupPlayerInputComponent();
}

