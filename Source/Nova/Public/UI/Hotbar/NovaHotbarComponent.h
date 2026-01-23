// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"
#include "NovaHotbarComponent.generated.h"

class UNovaAbilitySystemComponent;

/**
 *	핫바 슬롯 데이터 구조체
 */
USTRUCT(BlueprintType)
struct FNovaHotbarSlot
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hotbar")
	FGameplayTag AbilityTag;

	bool IsValid() const { return AbilityTag.IsValid(); }

	bool operator==(const FNovaHotbarSlot& Other) const { return AbilityTag == Other.AbilityTag; }
	bool operator!=(const FNovaHotbarSlot& Other) const { return !(*this == Other); }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNovaHotbarSlotsChanged);

/**
 *	Author: 지용현
 *	Date: 2026.01.21
 *	플레이어의 핫바(단축바) 데이터를 관리하는 컴포넌트
 *	PlayerController에 부착되어 사용됩니다. (로컬 전용)
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class NOVA_API UNovaHotbarComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UNovaHotbarComponent(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable, Category = "Nova|Hotbar")
	void SetSlotAbility(int32 SlotIndex, FGameplayTag AbilityTag);

	UFUNCTION(BlueprintCallable, Category = "Nova|Hotbar")
	void ClearSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Nova|Hotbar")
	void UseSlot(int32 SlotIndex);

	UFUNCTION(BlueprintPure, Category = "Nova|Hotbar")
	const TArray<FNovaHotbarSlot>& GetSlots() const { return Slots; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nova|Hotbar")
	TArray<FNovaHotbarSlot> Slots;

	UPROPERTY(EditDefaultsOnly, Category = "Nova|Hotbar")
	int32 DefaultSlotCount = 10;

public:
	UPROPERTY(BlueprintAssignable, Category = "Nova|Hotbar")
	FNovaHotbarSlotsChanged OnSlotsChanged;
};