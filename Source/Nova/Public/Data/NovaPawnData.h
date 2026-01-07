// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "NovaPawnData.generated.h"

class UNovaInputConfig;

/**
 *	Author: 지용현
 *	Date: 2026.01.06(2026.01.06) 
 *	Pawn 설정 데이터 관리
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "Nova Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class NOVA_API UNovaPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UNovaPawnData(const FObjectInitializer& ObjectInitializer);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Nova|Input")
	TObjectPtr<UNovaInputConfig> InputConfig;
};
