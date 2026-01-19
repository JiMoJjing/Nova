// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "TargetIndicator.generated.h"

/**
 *	Author: 지용현
 *	Date: 2026.01.15
 *
 *	[Target Indicator Actor]
 *	하드 타겟팅(Hard Targeting) 시스템에서 현재 선택된 대상을 시각적으로 강조하는 액터입니다.
 *	
 *	주요 기능:
 *	1. 시각적 피드백 제공: 대상의 머리 위에는 위젯(MarkWidget)을, 발 밑에는 데칼(GroundDecal)을 렌더링합니다.
 *	2. 동적 크기/위치 보정: ITargetableInterface를 통해 대상의 콜리전 크기(Bounds)를 조회하여,
 *	   대상의 키에 맞춰 위젯 높이를 조절하고 대상의 반지름에 맞춰 데칼 크기를 자동으로 스케일링합니다.
 *	3. 최적화: 활성화 시에만 대상에 부착(Attach)되어 따라다니며, 비활성화 시에는 Tick과 렌더링이 꺼집니다.
 *	
 *	참고:
 *	이 액터는 로컬 플레이어 컨트롤러(HardTargetingComponent)에 의해 클라이언트에서만 생성 및 관리됩니다.
 */

class UWidgetComponent;
class UDecalComponent;

UCLASS()
class HARDTARGETINGSYSTEM_API ATargetIndicator : public AActor
{
	GENERATED_BODY()

public:
	ATargetIndicator();

protected:
	virtual void BeginPlay() override;
	void Init();

	void Activate(AActor* NewTarget);
	void Deactivate();
	
	void SetIndicatorOffset();
	void SetColorByFactionTag(const FGameplayTag& TargetFactionTag);

public:
	virtual void Tick(float DeltaTime) override;

	void SetTarget(AActor* NewTarget);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UWidgetComponent> MarkWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TObjectPtr<UDecalComponent> GroundDecal;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FVector MarkWidgetOffset = FVector(0.0f, 0.0f, 40.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FVector GroundDecalOffset = FVector(0.0f, 0.0f, 0.0f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float DefaultDecalSizeOffset = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TMap<FGameplayTag, FLinearColor> MarkColors;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	TMap<FGameplayTag, FColor> DecalColors;
		
	UPROPERTY()
	TWeakObjectPtr<AActor> CurrentTarget;
	
	UPROPERTY()
	TWeakObjectPtr<UMaterialInstanceDynamic> DecalMaterial;
	
	UPROPERTY()
	TWeakObjectPtr<UUserWidget> MarkWidgetObject;
};
