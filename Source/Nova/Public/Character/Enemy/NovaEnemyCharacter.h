// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/NovaCharacterBase.h"
#include "NovaEnemyCharacter.generated.h"

UCLASS()
class NOVA_API ANovaEnemyCharacter : public ANovaCharacterBase
{
	GENERATED_BODY()

public:
	ANovaEnemyCharacter();

protected:
	virtual void BeginPlay() override;

};
