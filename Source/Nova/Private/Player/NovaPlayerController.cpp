// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/NovaPlayerController.h"

#include "EnhancedInputSubsystems.h"
#include "Components/HardTargetingComponent.h"
#include "GAS/NovaAbilitySystemComponent.h"
#include "Input/NovaInputComponent.h"
#include "Interfaces/TargetableInterface.h"
#include "Player/NovaPlayerState.h"

ANovaPlayerController::ANovaPlayerController(const FObjectInitializer& ObjectInitializer)
{
	// 로컬 컨트롤러에서만 작동해야 하는 컴포넌트이므로 생성 시점을 변경.
	// HardTargetingComponent = CreateDefaultSubobject<UHardTargetingComponent>(TEXT("HardTargetingComponent"));

	bReplicates = true;

	bEnableClickEvents = false; 
	bEnableMouseOverEvents = false;
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
	
	// FInputModeGameAndUI InputModeData;
	// InputModeData.SetHideCursorDuringCapture(false); 
	// InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	// InputModeData.SetWidgetToFocus(nullptr);
	//
	// SetInputMode(InputModeData);

	if (IsLocalController())
	{
		HardTargetingComponent = NewObject<UHardTargetingComponent>(this, TEXT("HardTargetingComponent"));

		if (HardTargetingComponent != nullptr)
		{
			HardTargetingComponent->RegisterComponent();

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
		Subsystem->AddMappingContext(PlayerControllerMappingContext, 1);
	}

	UNovaInputComponent* NovaIC = Cast<UNovaInputComponent>(InputComponent);
	if (NovaIC != nullptr)
	{
		NovaIC->BindAction(LeftMouseClickAction, ETriggerEvent::Completed, this, &ANovaPlayerController::ClickTarget);
		NovaIC->BindAction(RightMouseClickAction, ETriggerEvent::Completed, this, &ANovaPlayerController::ClickTarget);
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
	GEngine->AddOnScreenDebugMessage(3, 3.0f, FColor::Green, TEXT("ClickTarget!"));

	if (HardTargetingComponent != nullptr)
	{
		HardTargetingComponent->SelectTargetUnderCursor();
	}
}

void ANovaPlayerController::OnHoveredTargetChanged(AActor* NewTarget, AActor* OldTarget)
{
	
}

void ANovaPlayerController::OnCurrentTargetChanged(AActor* NewTarget, AActor* OldTarget)
{
	
}
