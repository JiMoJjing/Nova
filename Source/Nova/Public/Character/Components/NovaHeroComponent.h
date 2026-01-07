// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NovaHeroComponent.generated.h"


class USpringArmComponent;
class UInputAction;
class UInputMappingContext;
class UInputComponent;
struct FGameplayTag;
struct FInputActionValue;

/**
 *	Author: 지용현
 *	Date: 2026.01.04(2026.01.06) 
 *	캐릭터 기본 움직임, 카메라 줌, Tag기반 Ability입력
 *	기본 입력 매핑 관리
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class NOVA_API UNovaHeroComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNovaHeroComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void OnRegister() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
protected:
	void HandleChangeInit();
	void InitializePlayerInput(UInputComponent* PlayerInputComponent);
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	void Input_Zoom(const FInputActionValue& InputActionValue);
	void Input_Jump(const FInputActionValue& InputActionValue);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nova|Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nova|Camera", meta = (AllowPrivateAccess = "true"))
	float ZoomSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nova|Camera", meta = (AllowPrivateAccess = "true"))
	float MinZoomLength;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nova|Camera", meta = (AllowPrivateAccess = "true"))
	float MaxZoomLength;

private:
	UPROPERTY()
	TObjectPtr<USpringArmComponent> SpringArm;

	float CurrentZoomLength;
};