// Copyright Epic Games, Inc. All Rights Reserved.

#include "Hallucinations.h"

#include "Internationalization/StringTableRegistry.h"
#include "Modules/ModuleManager.h"

IMPLEMENT_PRIMARY_GAME_MODULE( FHallucinations, Hallucinations, "Hallucinations" );

void FHallucinations::StartupModule()
{
#if WITH_EDITOR
	if (!FStringTableRegistry::Get().FindStringTable("GameplayEventMessages"))
	{
		LOCTABLE_FROMFILE_GAME("GameplayEventMessages", "GameplayEventMessages", "Hallucinations/UI/StringTables/GameplayEventMessages.csv");
	}
#else
	LOCTABLE_FROMFILE_GAME("GameplayEventMessages", "GameplayEventMessages", "Hallucinations/UI/StringTables/GameplayEventMessages.csv");
#endif
}
