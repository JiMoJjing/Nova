// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NovaPawnExtensionComponent.generated.h"

class UNovaAbilitySystemComponent;
class UNovaPawnData;

/**
 *	Author: 지용현
 *	Date: 2026.01.04(2026.01.07)
 *	Pawn이 정상 동작하기 위해 필요한 외부 조건들이 모두 충족되었을 때만, 관련 컴포넌트들의 초기화를 보장한다.
 *	(PlayerController, PlayerState, InputComponent, etc...)
 *	Pawn의 초기화를 위한 데이터 관리
 */
UCLASS(Blueprintable, BlueprintType, meta=(BlueprintSpawnableComponent))
class NOVA_API UNovaPawnExtensionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UNovaPawnExtensionComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	static UNovaPawnExtensionComponent* FindNovaPawnExtensionComponent(const AActor* Actor);

	const UNovaPawnData* GetPawnData() const { return PawnData; }
	
	UNovaAbilitySystemComponent* GetNovaAbilitySystemComponent() const { return AbilitySystemComponent; }
	
	void CheckDefaultInitialization();
	
	void InitializeAbilitySystem(UNovaAbilitySystemComponent* InASC, AActor* InOwnerActor);
	void UninitializeAbilitySystem();

	void HandleControllerChanged();
	void HandlePlayerStateReplicated();
	void SetupPlayerInputComponent();

	FSimpleMulticastDelegate OnPawnReadyToInitialize;
	FSimpleMulticastDelegate OnAbilitySystemInitialized;
	FSimpleMulticastDelegate OnAbilitySystemUninitialized;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nova|Pawn", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNovaPawnData> PawnData;

	UPROPERTY(Transient)
	UNovaAbilitySystemComponent* AbilitySystemComponent;
	
	uint8 bIsDefaultInitialized : 1 = false;
};