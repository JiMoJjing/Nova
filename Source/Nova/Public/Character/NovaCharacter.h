// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Character.h"
#include "Interfaces/TargetableInterface.h"
#include "Logging/LogMacros.h"
#include "NovaCharacter.generated.h"

class UNovaPawnExtensionComponent;
class UNovaHeroComponent;
class USpringArmComponent;
class UCameraComponent;

/**
 *	Author: 지용현
 *	Date: 2026.01.04(2026.01.04)
 *	프로젝트의 플레이어 캐릭터
 */
UCLASS(config=Game)
class ANovaCharacter : public ACharacter, public ITargetableInterface
{
	GENERATED_BODY()

public:
	ANovaCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	void OnFactionTagChanged();
	
public:
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:
	virtual void OnHovered() override;
	virtual void OnUnhovered() override;
	virtual void OnSelected() override;
	virtual void OnDeselected() override;
	virtual void GetTargetBounds(float& OutHalfWidth, float& OutHalfHeight) const override;
	virtual FGameplayTag GetFactionTag() const override;
	virtual void SetFactionTag(const FGameplayTag& NewFactionTag) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nova", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNovaPawnExtensionComponent> NovaPawnExtensionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nova", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNovaHeroComponent> NovaHeroComponent;

	UPROPERTY()
	int32 OutlineStencilValue = 255;

	uint8 bIsSelected : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Nova|Team", meta = (AllowPrivateAccess = "true", Categories = "Faction"))
	FGameplayTag FactionTag;
};
