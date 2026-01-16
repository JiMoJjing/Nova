// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HardTargetingComponent.generated.h"

/**
 *	Author: 지용현
 *	Date: 2026.01.08
 *
 *	MMORPG 스타일의 하드 타겟팅(Hard Targeting) 시스템을 구현한 액터 컴포넌트입니다.
 *	마우스 커서 위치를 기반으로 대상을 탐지(Trace)하며, 호버링(Hover) 및 타겟 고정(Lock-On) 기능을 제공합니다.
 *	Tick 최적화를 통해 상태 변화가 감지될 때만 델리게이트를 방송하며, 
 *	ITargetableInterface를 구현한 액터만을 대상으로 필터링합니다.
 */

class ATargetIndicator;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCurrentTargetChanged, AActor*, NewTarget, AActor*, OldTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHoveredTargetChanged, AActor*, NewTarget, AActor*, OldTarget);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HARDTARGETINGSYSTEM_API UHardTargetingComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHardTargetingComponent(const FObjectInitializer& ObjectInitializer);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void SelectTargetUnderCursor();
	void ClearTarget();

	UFUNCTION(BlueprintPure)
	AActor* GetCurrentTarget() const { return CurrentTarget.Get(); }

protected:
	void PerformTargeting(FHitResult& OutHitResult);
	void UpdateHoveredTarget(const FHitResult& OutHitResult);

	void ChangeCurrentTarget(AActor* NewTarget);

	UFUNCTION(Server, Reliable)
	void Server_SetCurrentTarget(AActor* NewTarget);
	
	void SetIndicatorTarget(AActor* NewTarget);

public:
	UPROPERTY(BlueprintAssignable)
	FOnCurrentTargetChanged OnCurrentTargetChanged;

	UPROPERTY(BlueprintAssignable)
	FOnHoveredTargetChanged OnHoveredTargetChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HardTargetingSystem", meta = (AllowPrivateAccess = "true"))
	float TraceDistance = 10000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HardTargetingSystem", meta = (AllowPrivateAccess = "true"))
	TEnumAsByte<ECollisionChannel> TraceChannel = ECC_Visibility;
	
	UPROPERTY(Replicated, Transient)
	TObjectPtr<AActor> CurrentTarget;

	UPROPERTY()
	TWeakObjectPtr<AActor> HoveredTarget;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HardTargetingSystem", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATargetIndicator> TargetIndicatorClass;
	
	UPROPERTY()
	TObjectPtr<ATargetIndicator> TargetIndicator;
};
