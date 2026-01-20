// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Enemy/NovaEnemyCharacter.h"

#include "NovaGameplayTags.h"


ANovaEnemyCharacter::ANovaEnemyCharacter()
{
	FactionTag = NovaGameplayTags::Faction_Enemy;
}

void ANovaEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}
