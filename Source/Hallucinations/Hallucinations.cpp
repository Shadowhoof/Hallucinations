// Copyright Epic Games, Inc. All Rights Reserved.

#include "Hallucinations.h"

#include "Internationalization/StringTableRegistry.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FHallucinations, Hallucinations, "Hallucinations" );

void FHallucinations::StartupModule()
{
	LOCTABLE_FROMFILE_GAME("GameplayEventMessages", "GameplayEventMessages", "Hallucinations/UI/StringTables/GameplayEventMessages.csv");
}
