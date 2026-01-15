
#include "Components/HardTargetingComponent.h"
#include "Interfaces/TargetableInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Net/UnrealNetwork.h"


UHardTargetingComponent::UHardTargetingComponent(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);
}

void UHardTargetingComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UHardTargetingComponent, CurrentTarget);
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
		}
	}
}

void UHardTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APlayerController* PC = Cast<APlayerController>(GetOwner());

	if (PC && PC->IsLocalController() == true)
	{
		FHitResult HitResult;
		PerformTargeting(HitResult);
		UpdateHoveredTarget(HitResult);	
	}
}

void UHardTargetingComponent::SelectTargetUnderCursor()
{
	AActor* NewTarget = HoveredTarget.Get();
	
	ChangeCurrentTarget(NewTarget);

	if (GetOwner()->HasAuthority() == false)
	{
		Server_SetCurrentTarget(NewTarget);
	}
}

void UHardTargetingComponent::ClearTarget()
{
	ChangeCurrentTarget(nullptr);

	if (GetOwner()->HasAuthority() == false)
	{
		Server_SetCurrentTarget(nullptr);
	}
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

void UHardTargetingComponent::ChangeCurrentTarget(AActor* NewTarget)
{
	if (CurrentTarget == NewTarget)
	{
		return;
	}

	AActor* OldTarget = CurrentTarget;

	if (OldTarget != nullptr)
	{
		if (ITargetableInterface* OldTargetInterface = Cast<ITargetableInterface>(OldTarget))
		{
			OldTargetInterface->OnDeselected();
		}
	}

	CurrentTarget = NewTarget;

	if (CurrentTarget != nullptr)
	{
		if (ITargetableInterface* NewTargetInterface = Cast<ITargetableInterface>(CurrentTarget))
		{
			NewTargetInterface->OnSelected();
		}
	}
	
	OnCurrentTargetChanged.Broadcast(CurrentTarget, OldTarget);
}

void UHardTargetingComponent::Server_SetCurrentTarget_Implementation(AActor* NewTarget)
{
	CurrentTarget = NewTarget;
}
