// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TargetIndicator.h"

#include "Components/DecalComponent.h"
#include "Components/WidgetComponent.h"
#include "Interfaces/TargetableInterface.h"

ATargetIndicator::ATargetIndicator()
{
	PrimaryActorTick.bCanEverTick = true;
	
	bReplicates = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	RootComponent = SceneRoot;
	
	MarkWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("MarkWidget"));
	MarkWidget->SetupAttachment(SceneRoot);
	MarkWidget->SetWidgetSpace(EWidgetSpace::World);
	MarkWidget->SetDrawAtDesiredSize(false);
	MarkWidget->SetDrawSize(FVector2D(24.0f, 48.0f));
	
	GroundDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("GroundDecal"));
	GroundDecal->SetupAttachment(SceneRoot);
	GroundDecal->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	GroundDecal->DecalSize = FVector(100.0f, 100.0f, 100.0f);;
}

void ATargetIndicator::BeginPlay()
{
	Super::BeginPlay();

	Deactivate();
}

void ATargetIndicator::Activate(AActor* NewTarget)
{
	if (NewTarget == nullptr)
	{
		return;
	}

	CurrentTarget = NewTarget;
	AttachToActor(CurrentTarget.Get(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	
	SetIndicatorOffset();
	
	SetActorTickEnabled(true);	
	SetActorHiddenInGame(false);
}

void ATargetIndicator::Deactivate()
{
	SetActorTickEnabled(false);	
	SetActorHiddenInGame(true);
	
	if (CurrentTarget.IsValid())
	{
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
	
	CurrentTarget = nullptr;
}

void ATargetIndicator::SetIndicatorOffset()
{
	ITargetableInterface* TargetInterface = Cast<ITargetableInterface>(CurrentTarget.Get());
	if (TargetInterface == nullptr)
	{
		return;
	}
	
	float HalfWidth = 0.0f;
	float HalfHeight = 0.0f;
	
	TargetInterface->GetTargetBounds(HalfWidth, HalfHeight);
	
	HalfWidth = HalfWidth == 0.0f ? 1.0f : HalfWidth;
	HalfHeight = HalfHeight == 0.0f ? 1.0f : HalfHeight;
	
	const FVector NewWidgetLocation = FVector(0.0f, 0.0f, HalfHeight);
	MarkWidget->SetRelativeLocation(NewWidgetLocation + MarkWidgetOffset);
	
	const FVector NewDecalLocation = FVector(0.0f, 0.0f, -HalfHeight);
	GroundDecal->SetRelativeLocation(NewDecalLocation + GroundDecalOffset);
	
	const float DecalSizeRatio = HalfWidth / DefaultDecalSizeOffset;
	const FVector NewDecalScale = FVector(1.0f, 1.0f, 1.0f) * DecalSizeRatio;
	
	GroundDecal->SetWorldScale3D(NewDecalScale);
}

void ATargetIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (MarkWidget != nullptr)
	{
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC != nullptr)
		{
			const FVector CameraLocation = PC->PlayerCameraManager->GetCameraLocation();
			const FVector WidgetLocation = MarkWidget->GetComponentLocation();
			
			const FRotator LookAtRotation = (CameraLocation - WidgetLocation).Rotation();
			
			MarkWidget->SetWorldRotation(LookAtRotation);
		}
	}
}

void ATargetIndicator::SetTarget(AActor* NewTarget)
{
	if (CurrentTarget.Get() == NewTarget)
	{
		return;
	}
	
	if (NewTarget == nullptr)
	{
		Deactivate();
		return;
	}
	
	Activate(NewTarget);
}
