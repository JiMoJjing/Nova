// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Engine/DataAsset.h"
#include "NovaAbilitySet.generated.h"


class UNovaAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FNovaAbilitySetItem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, category = "Ability")
	TSubclassOf<UGameplayAbility> Ability = nullptr;
	
	UPROPERTY(EditDefaultsOnly, category = "Ability", meta = (Categories = "Ability"))
	FGameplayTag AbilityTag;

	UPROPERTY(EditDefaultsOnly, category = "UI")
	TObjectPtr<UTexture2D> Icon = nullptr;

	UPROPERTY(EditDefaultsOnly, category = "UI", meta = (MultiLine = "true"))
	FText Description;

	UPROPERTY(EditDefaultsOnly, category = "Input", meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
};

/**
 *	Author: 지용현
 *	Date: 2026.01.21
 *	GAS 어빌리티 부여 데이터 정의 (아이콘, 실행 태그 포함)
 */

UCLASS(BlueprintType, Const)
class NOVA_API UNovaAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UNovaAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	void GiveToAbilitySystem(UNovaAbilitySystemComponent* NovaASC, TArray<FGameplayAbilitySpecHandle>& OutGrantedHandles, UObject* SourceObject = nullptr) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities", meta=(TitleProperty="Ability"))
	TArray<FNovaAbilitySetItem> GrantedAbilities;
};
