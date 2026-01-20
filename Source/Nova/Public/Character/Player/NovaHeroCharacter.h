// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/NovaCharacterBase.h"
#include "NovaHeroCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UNovaPawnExtensionComponent;
class UNovaHeroComponent;

/**
 *	Author: 지용현
 *	Date: 2026.01.20(2026.01.20)
 *	프로젝트의 플레이어 캐릭터
 *	
 */

UCLASS()
class NOVA_API ANovaHeroCharacter : public ANovaCharacterBase
{
	GENERATED_BODY()

public:
	ANovaHeroCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	virtual void OnRep_Controller() override;
	virtual void OnRep_PlayerState() override;

public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nova", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNovaPawnExtensionComponent> NovaPawnExtensionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Nova", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNovaHeroComponent> NovaHeroComponent;
};
