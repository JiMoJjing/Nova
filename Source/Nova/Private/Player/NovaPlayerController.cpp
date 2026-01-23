// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NovaPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Components/HardTargetingComponent.h"
#include "GAS/NovaAbilitySystemComponent.h"
#include "Input/NovaInputComponent.h"
#include "Player/NovaPlayerState.h"
#include "UI/Hotbar/NovaHotbarComponent.h"

ANovaPlayerController::ANovaPlayerController(const FObjectInitializer& ObjectInitializer)
{
	HardTargetingComponent = CreateDefaultSubobject<UHardTargetingComponent>(TEXT("HardTargetingComponent"));
	HotbarComponent = CreateDefaultSubobject<UNovaHotbarComponent>(TEXT("HotbarComponent"));

	bReplicates = true;

}

ANovaPlayerState* ANovaPlayerController::GetNovaPlayerState() const
{
	return CastChecked<ANovaPlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UNovaAbilitySystemComponent* ANovaPlayerController::GetNovaAbilitySystemComponent() const
{
	const ANovaPlayerState* NovaPS = GetNovaPlayerState();
	if (NovaPS)
	{
		return NovaPS->GetNovaAbilitySystemComponent();
	}

	return nullptr;
}

void ANovaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;

	if (IsLocalController())
	{
		if (HardTargetingComponent != nullptr)
		{
			HardTargetingComponent->OnHoveredTargetChanged.AddDynamic(this, &ANovaPlayerController::OnHoveredTargetChanged);
			HardTargetingComponent->OnCurrentTargetChanged.AddDynamic(this, &ANovaPlayerController::OnCurrentTargetChanged);
		}
	}
}

void ANovaPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ANovaPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	ULocalPlayer* LP = GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	if (PlayerControllerMappingContext != nullptr)
	{
		Subsystem->AddMappingContext(PlayerControllerMappingContext, 0);
	}

	UNovaInputComponent* NovaIC = Cast<UNovaInputComponent>(InputComponent);
	if (NovaIC != nullptr)
	{
		NovaIC->BindAction(LeftMouseClickAction, ETriggerEvent::Completed, this, &ANovaPlayerController::ClickTarget);
		NovaIC->BindAction(RightMouseClickAction, ETriggerEvent::Completed, this, &ANovaPlayerController::ClickTarget);
		
		if (QuickSlotInputConfig != nullptr)
		{
			TArray<uint32> Handles;
			NovaIC->BindAbilityActions(QuickSlotInputConfig, this, &ANovaPlayerController::Input_AbilityInputTagPressed, &ANovaPlayerController::Input_AbilityInputTagReleased, Handles);
		}
	}
}

void ANovaPlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UNovaAbilitySystemComponent* NovaASC = GetNovaAbilitySystemComponent())
	{
		NovaASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);
}

void ANovaPlayerController::ClickTarget()
{
	if (HardTargetingComponent != nullptr)
	{
		HardTargetingComponent->SelectTargetUnderCursor();
	}
}

void ANovaPlayerController::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	const int32 SlotIndex = GetHotbarSlotIndexFromTag(InputTag);

	if (SlotIndex >= 0)
	{
		if (HotbarComponent)
		{
			HotbarComponent->UseSlot(SlotIndex);
		}
	}
	else
	{
		if (UNovaAbilitySystemComponent* NovaASC = GetNovaAbilitySystemComponent())
		{
			NovaASC->AbilityInputTagPressed(InputTag);
		}
	}
}

void ANovaPlayerController::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (UNovaAbilitySystemComponent* NovaASC = GetNovaAbilitySystemComponent())
	{
		NovaASC->AbilityInputTagReleased(InputTag);
	}
}

int32 ANovaPlayerController::GetHotbarSlotIndexFromTag(FGameplayTag InputTag)
{
	FString TagString = InputTag.ToString();
	
	if (TagString.Contains(TEXT("QuickSlot")))
	{
		int32 LastDotIndex;
		
		if (TagString.FindLastChar('.', LastDotIndex))
		{
			FString NumberStr = TagString.RightChop(LastDotIndex + 1);
			if (NumberStr.IsNumeric())
			{
				int32 SlotNum = FCString::Atoi(*NumberStr);
				return SlotNum > 0 ? SlotNum - 1 : -1;
			}
		}
	}

	return -1;
}

void ANovaPlayerController::OnHoveredTargetChanged(AActor* NewTarget, AActor* OldTarget)
{
	
}

void ANovaPlayerController::OnCurrentTargetChanged(AActor* NewTarget, AActor* OldTarget)
{
	
}
