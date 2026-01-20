// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "Interfaces/TargetableInterface.h"
#include "NovaCharacterBase.generated.h"

/**
 *	Author: 지용현
 *	Date: 2026.01.04(2026.01.20)
 *	프로젝트의 캐릭터 베이스
 *	플레이어와 몬스터로 파생
 */
UCLASS(config=Game)
class ANovaCharacterBase : public ACharacter, public ITargetableInterface
{
	GENERATED_BODY()

public:
	ANovaCharacterBase();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void OnFactionTagChanged();

public:
	virtual void OnHovered() override;
	virtual void OnUnhovered() override;
	virtual void OnSelected() override;
	virtual void OnDeselected() override;
	virtual void GetTargetBounds(float& OutHalfWidth, float& OutHalfHeight) const override;
	virtual FGameplayTag GetFactionTag() const override;
	virtual void SetFactionTag(const FGameplayTag& NewFactionTag) override;

protected:
	UPROPERTY()
	int32 OutlineStencilValue = 255;

	uint8 bIsSelected : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nova|Team", meta = (AllowPrivateAccess = "true", Categories = "Faction"))
	FGameplayTag FactionTag;
};
