// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "NovaPlayerController.generated.h"

class UNovaInputConfig;
class UNovaHotbarComponent;
class UInputAction;
class UInputMappingContext;
class UHardTargetingComponent;
class ANovaPlayerState;
class UNovaAbilitySystemComponent;
/**
 *	Author: 지용현
 *	Date: 26.01.07(26.01.07) 
 *	NovaASC의 Tag입력 처리를 위한 PostProcessInput 오버라이드
 */
UCLASS()
class NOVA_API ANovaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ANovaPlayerController(const FObjectInitializer& ObjectInitializer);

	ANovaPlayerState* GetNovaPlayerState() const;

	UNovaAbilitySystemComponent* GetNovaAbilitySystemComponent() const;
	
	UFUNCTION(BlueprintCallable)
	UHardTargetingComponent* GetHardTargetingComponent() const { return HardTargetingComponent; }
	
	UFUNCTION(BlueprintCallable)
	UNovaHotbarComponent* GetHotbarComponent() const { return HotbarComponent; }

protected:
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void SetupInputComponent() override;
	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	void ClickTarget();
	
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);
	
	int32 GetHotbarSlotIndexFromTag(FGameplayTag InputTag);

	UFUNCTION()
	void OnHoveredTargetChanged(AActor* NewTarget, AActor* OldTarget);

	UFUNCTION()
	void OnCurrentTargetChanged(AActor* NewTarget, AActor* OldTarget);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nova", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHardTargetingComponent> HardTargetingComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nova", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNovaHotbarComponent> HotbarComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nova|Targeting", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> PlayerControllerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nova|Targeting", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LeftMouseClickAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nova|Targeting", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> RightMouseClickAction;	
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nova|Input")
	TObjectPtr<UNovaInputConfig> QuickSlotInputConfig;
};
