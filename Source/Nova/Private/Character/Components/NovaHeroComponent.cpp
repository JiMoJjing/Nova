// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Components/NovaHeroComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameplayTagContainer.h"
#include "NovaGameplayTags.h"
#include "Character/Components/NovaPawnExtensionComponent.h"
#include "Data/NovaPawnData.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/NovaAbilitySystemComponent.h"
#include "Input/NovaInputComponent.h"


UNovaHeroComponent::UNovaHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ZoomSpeed = 20.0f;
	MinZoomLength = 100.0f;
	MaxZoomLength = 1000.0f;
	CurrentZoomLength = 100.0f;
}

void UNovaHeroComponent::OnRegister()
{
	Super::OnRegister();

	UNovaPawnExtensionComponent* PawnExtComp = UNovaPawnExtensionComponent::FindNovaPawnExtensionComponent(GetOwner());
	if (PawnExtComp)
	{
		PawnExtComp->OnPawnReadyToInitialize.AddUObject(this, &UNovaHeroComponent::HandleChangeInit);
	}
}

void UNovaHeroComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UNovaHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UNovaHeroComponent::HandleChangeInit()
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (Pawn == nullptr)
	{
		return;
	}
	
	SpringArm = Pawn->FindComponentByClass<USpringArmComponent>();
	if (SpringArm != nullptr)
	{
		CurrentZoomLength = SpringArm->TargetArmLength;
	}

	if (Pawn->InputComponent)
	{
		InitializePlayerInput(Pawn->InputComponent);
	}
}

void UNovaHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	if (PlayerInputComponent == nullptr)
	{
		return;
	}
	
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (Pawn == nullptr)
	{
		return;
	}

	APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
	check(PC);

	ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(DefaultInputMappingContext, 0);

	if (const UNovaPawnExtensionComponent* PawnExtComp = UNovaPawnExtensionComponent::FindNovaPawnExtensionComponent(Pawn))
	{
		if (const UNovaPawnData* PawnData = PawnExtComp->GetPawnData())
		{
			if (const UNovaInputConfig* InputConfig = PawnData->InputConfig)
			{
				if (UNovaInputComponent* NovaIC = Cast<UNovaInputComponent>(PlayerInputComponent))
				{
					TArray<uint32> BindHandles;
					NovaIC->BindAbilityActions(InputConfig, this, &UNovaHeroComponent::Input_AbilityInputTagPressed, &UNovaHeroComponent::Input_AbilityInputTagReleased, BindHandles);
					
					NovaIC->BindNativeAction(InputConfig, NovaGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &UNovaHeroComponent::Input_Move);
					NovaIC->BindNativeAction(InputConfig, NovaGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &UNovaHeroComponent::Input_Look);
					NovaIC->BindNativeAction(InputConfig, NovaGameplayTags::InputTag_Zoom, ETriggerEvent::Triggered, this, &UNovaHeroComponent::Input_Zoom);
					NovaIC->BindNativeAction(InputConfig, NovaGameplayTags::InputTag_Jump, ETriggerEvent::Started, this, &UNovaHeroComponent::Input_Jump);
				}				
			}
		}
	}
}

void UNovaHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = Cast<APawn>(GetOwner()))
	{
		if (const UNovaPawnExtensionComponent* PawnExtComp = UNovaPawnExtensionComponent::FindNovaPawnExtensionComponent(Pawn))
		{
			if (UNovaAbilitySystemComponent* NovaASC = PawnExtComp->GetNovaAbilitySystemComponent())
			{
				NovaASC->AbilityInputTagPressed(InputTag);
			}
		}
	}
}

void UNovaHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (const APawn* Pawn = Cast<APawn>(GetOwner()))
	{
		if (const UNovaPawnExtensionComponent* PawnExtComp = UNovaPawnExtensionComponent::FindNovaPawnExtensionComponent(Pawn))
		{
			if (UNovaAbilitySystemComponent* NovaASC = PawnExtComp->GetNovaAbilitySystemComponent())
			{
				NovaASC->AbilityInputTagReleased(InputTag);
			}
		}
	}
}

void UNovaHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (Pawn == nullptr)
	{
		return;
	}
	AController* Controller = Pawn->GetController();
	if (Controller == nullptr)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();
	const FRotator YawRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	Pawn->AddMovementInput(ForwardDirection, Value.Y);
	Pawn->AddMovementInput(RightDirection, Value.X);
}

void UNovaHeroComponent::Input_Look(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (Pawn == nullptr)
	{
		return;
	}
	
	FVector2D Value = InputActionValue.Get<FVector2D>();
	Pawn->AddControllerYawInput(Value.X);
	Pawn->AddControllerPitchInput(Value.Y);
}

void UNovaHeroComponent::Input_Zoom(const FInputActionValue& InputActionValue)
{
	if (SpringArm == nullptr)
	{
		return;
	}

	float Value = InputActionValue.Get<float>();
	CurrentZoomLength += (Value * ZoomSpeed * -1.0f);
	CurrentZoomLength = FMath::Clamp(CurrentZoomLength, MinZoomLength, MaxZoomLength);
	SpringArm->TargetArmLength = CurrentZoomLength;
}

void UNovaHeroComponent::Input_Jump(const FInputActionValue& InputActionValue)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	if (Character != nullptr)
	{
		Character->Jump();
	}
}