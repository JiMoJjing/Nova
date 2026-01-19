// Copyright Epic Games, Inc. All Rights Reserved.

#include "NovaGameplayTags.h"

#include "GameplayTagsManager.h"
#include "Engine/EngineTypes.h"

/**
 *
 *
 * 2026.01.06(2026.01.06)
 */
namespace NovaGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Move, "InputTag.Move", "Move Input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Look, "InputTag.Look", "Look Input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Zoom, "InputTag.Zoom", "Zoom Input.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(InputTag_Jump, "InputTag.Jump", "Jump Input.");
	
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Faction_Player, "Faction.Player", "Faction player.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Faction_Enemy, "Faction.Enemy", "Faction Enemy.");
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(Faction_NPC, "Faction.NPC", "Faction NPC.");
	
}