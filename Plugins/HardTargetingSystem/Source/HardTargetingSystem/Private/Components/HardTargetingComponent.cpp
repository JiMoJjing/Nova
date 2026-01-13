
#include "Components/HardTargetingComponent.h"
#include "Interfaces/TargetableInterface.h"
#include "Kismet/KismetSystemLibrary.h"


UHardTargetingComponent::UHardTargetingComponent(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHardTargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (PC)
	{
		if (PC->IsLocalController() == false)
		{
			SetComponentTickEnabled(false);
			Deactivate();
		}
	}
}

void UHardTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult HitResult;
	PerformTargeting(HitResult);
	UpdateHoveredTarget(HitResult);
}

void UHardTargetingComponent::SelectTargetUnderCursor()
{
	AActor* NewTarget = HoveredTarget.Get();
	
	if (NewTarget == nullptr)
	{
		ClearTarget();
		return;
	}
	
	if (CurrentTarget.Get() == NewTarget)
	{
		return;
	}
	
	AActor* OldTarget = CurrentTarget.Get();
	
	if (OldTarget != nullptr)
	{
		if (ITargetableInterface* OldTargetInterface = Cast<ITargetableInterface>(OldTarget))
		{
			OldTargetInterface->OnDeselected();
		}
	}

	CurrentTarget = NewTarget;
	
	if (ITargetableInterface* NewTargetInterface = Cast<ITargetableInterface>(NewTarget))
	{
		NewTargetInterface->OnSelected();
	}
	
	OnCurrentTargetChanged.Broadcast(CurrentTarget.Get(), OldTarget);
}

void UHardTargetingComponent::ClearTarget()
{
	if (CurrentTarget.Get() == nullptr)
	{
		return;
	}

	AActor* OldTarget = CurrentTarget.Get();
	
	if (ITargetableInterface* OldTargetInterface = Cast<ITargetableInterface>(OldTarget))
	{
		OldTargetInterface->OnDeselected();
	}

	CurrentTarget = nullptr;
   
	OnCurrentTargetChanged.Broadcast(nullptr, OldTarget);
}

void UHardTargetingComponent::PerformTargeting(FHitResult& OutHitResult)
{
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (PC == nullptr)
	{
		return;
	}

	FVector MouseLocation, MouseDirection;
	if (PC->DeprojectMousePositionToWorld(MouseLocation, MouseDirection) == true)
	{
		const FVector Start = MouseLocation;
		const FVector End = Start + (MouseDirection * TraceDistance);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(PC->GetPawn());
		QueryParams.bTraceComplex = false;

		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, TraceChannel, QueryParams);
	}
}

void UHardTargetingComponent::UpdateHoveredTarget(const FHitResult& OutHitResult)
{
	AActor* NewTarget = nullptr;

	if (OutHitResult.bBlockingHit == true)
	{
		AActor* HitActor = OutHitResult.GetActor();
		if (HitActor != nullptr)
		{
			if (ITargetableInterface* HitTargetInterface = Cast<ITargetableInterface>(HitActor))
			{
				NewTarget = HitActor;
			}
		}
	}

	if (HoveredTarget.Get() == NewTarget)
	{
		return;
	}

	AActor* OldTarget = HoveredTarget.Get();

	if (OldTarget != nullptr)
	{
		if (ITargetableInterface* OldTargetInterface = Cast<ITargetableInterface>(OldTarget))
		{
			OldTargetInterface->OnUnhovered();
		}
	}

	if (NewTarget != nullptr)
	{
		if (ITargetableInterface* NewTargetInterface = Cast<ITargetableInterface>(NewTarget))
		{
			NewTargetInterface->OnHovered();
		}
	}

	HoveredTarget = NewTarget;
	OnHoveredTargetChanged.Broadcast(NewTarget, OldTarget);
}