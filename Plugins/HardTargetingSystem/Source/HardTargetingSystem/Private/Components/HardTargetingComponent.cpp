
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

}

void UHardTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult HitResult;
	PerformTargeting(HitResult);
	UpdateHoveredTarget(HitResult);

	if (HoveredTarget.Get() != nullptr)
	{
		FString DisplayName = HoveredTarget.Get()->GetName();
		UKismetSystemLibrary::DrawDebugString(GetOwner(), HoveredTarget.Get()->GetActorLocation(), DisplayName, nullptr, FLinearColor::Blue, 0.01f);
	}
	else
	{
		if (HitResult.bBlockingHit == false)
		{
			return;
		}
		UKismetSystemLibrary::DrawDebugString(GetOwner(), HitResult.Location, TEXT("None"), nullptr, FLinearColor::Red, 0.01f);
	}
}

void UHardTargetingComponent::SelectTargetUnderCursor()
{
	if (HoveredTarget.Get() == nullptr)
	{
		ClearTarget();
		return;
	}
	
	if (CurrentTarget.Get() == HoveredTarget.Get())
	{
		return;
	}
	AActor* OldTarget = CurrentTarget.Get();
	CurrentTarget = HoveredTarget.Get();
	OnCurrentTargetChanged.Broadcast(CurrentTarget.Get(), OldTarget);
}

void UHardTargetingComponent::ClearTarget()
{
	if (CurrentTarget.Get() == nullptr)
	{
		return;
	}
	AActor* OldTarget = CurrentTarget.Get();
	CurrentTarget = nullptr;
	OnCurrentTargetChanged.Broadcast(CurrentTarget.Get(), OldTarget);
}

void UHardTargetingComponent::PerformTargeting(FHitResult& OutHitResult)
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (Pawn == nullptr)
	{
		return;
	}
	
	APlayerController* PC = Cast<APlayerController>(Pawn->GetController());
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
		QueryParams.AddIgnoredActor(Pawn);
		QueryParams.bTraceComplex = false;

		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, TraceChannel, QueryParams);
	}
}

void UHardTargetingComponent::UpdateHoveredTarget(const FHitResult& OutHitResult)
{
	AActor* OldTarget = HoveredTarget.Get();
	
	if (OutHitResult.bBlockingHit == true)
	{
		AActor* HitActor = OutHitResult.GetActor();
		if (HitActor != nullptr)
		{
			if (HoveredTarget.Get() == HitActor)
			{
				return;
			}
			
			if (ITargetableInterface* TargetableInterface = Cast<ITargetableInterface>(HitActor))
			{
				HoveredTarget = HitActor;
				OnHoveredTargetChanged.Broadcast(HitActor, OldTarget);
				return;
			}
		}
	}

	if (OldTarget == nullptr)
	{
		return;
	}
	
	HoveredTarget = nullptr;
	OnHoveredTargetChanged.Broadcast(nullptr, OldTarget);
}