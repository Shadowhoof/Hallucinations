// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Subsystems/Save/HSaveSubsystem.h"

#include "Core/Subsystems/Save/HSaveGame.h"
#include "Core/Subsystems/Save/HStatefulActorInterface.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogSave);

namespace SaveConstants
{
	const FString LevelPrefix = "Level:";
}


const FString UHSaveSubsystem::DefaultSlotName = "DefaultSlot";

void UHSaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadGame();
}

void UHSaveSubsystem::Deinitialize()
{
	SaveGame();

	// level data should not be saved between sessions
	DeleteLevelSaves();
	
	Super::Deinitialize();
}

UHSaveGame* UHSaveSubsystem::GetSaveData() const
{
	return SaveData;
}

UHLevelSave* UHSaveSubsystem::CreateLevelSave()
{
	return Cast<UHLevelSave>(CreateSave(UHLevelSave::StaticClass()));
}

void UHSaveSubsystem::SaveLevel(const UWorld* World, UHLevelSave* LevelSave)
{
	const FString LevelSlotName = GetLevelSlotName(World);
	Save(LevelSave, LevelSlotName, 0);
	VisitedLevelsSlotNames.Add(LevelSlotName);
}

UHLevelSave* UHSaveSubsystem::LoadLevel(const UWorld* World)
{
	return Cast<UHLevelSave>(Load(GetLevelSlotName(World), 0));
}

void UHSaveSubsystem::DeleteLevelSaves()
{
	for (const auto& SlotName : VisitedLevelsSlotNames)
	{
		if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
		{
			UE_LOG(LogSave, VeryVerbose, TEXT("Deleting level save %s"), *SlotName);
			UGameplayStatics::DeleteGameInSlot(SlotName, 0);
		}
	}
}

void UHSaveSubsystem::SaveGame()
{
	Save(SaveData, DefaultSlotName, 0);
}

void UHSaveSubsystem::LoadGame()
{
	SaveData = Cast<UHSaveGame>(LoadOrCreate(UHSaveGame::StaticClass(), DefaultSlotName, 0));
}

USaveGame* UHSaveSubsystem::CreateSave(TSubclassOf<USaveGame> SaveClass)
{
	return UGameplayStatics::CreateSaveGameObject(SaveClass);
}

void UHSaveSubsystem::Save(USaveGame* SaveGameObject, const FString& SlotName, int32 UserIndex)
{
	if (!SaveGameObject)
	{
		UE_LOG(LogSave, Warning, TEXT("Tried to save game to slot name %s with user index %d but save is empty"), *SlotName, UserIndex);
		return;
	}

	if (UGameplayStatics::SaveGameToSlot(SaveGameObject, SlotName, UserIndex))
	{
		UE_LOG(LogSave, Verbose, TEXT("Successfully saved to slot name %s with user index %d"), *SlotName, UserIndex);
	}
	else
	{
		UE_LOG(LogSave, Verbose, TEXT("Failed to save to slot name %s with user index %d"), *SlotName, UserIndex);
	}
}

USaveGame* UHSaveSubsystem::Load(const FString& SlotName, int32 UserIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		if (USaveGame* SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex))
		{
			UE_LOG(LogSave, Verbose, TEXT("Save loaded from slot %s with user index %d"), *SlotName, UserIndex);
			return SaveGameObject;
		}
		
		UE_LOG(LogSave, Verbose, TEXT("Save exists in slot %s with user index %d but it failed to load"), *SlotName, UserIndex);
		return nullptr;
	}

	UE_LOG(LogSave, Verbose, TEXT("No save found in slot %s with user index %d"), *SlotName, UserIndex);
	return nullptr;
}

USaveGame* UHSaveSubsystem::LoadOrCreate(TSubclassOf<USaveGame> SaveClass, const FString& SlotName, int32 UserIndex)
{
	USaveGame* SaveGameObject = Load(SlotName, UserIndex);

	if (!SaveGameObject)
	{
		UE_LOG(LogSave, Verbose, TEXT("No save found in slot %s with user index %d, creating a new one"), *SlotName, UserIndex);
		return CreateSave(SaveClass);
	}
	
	return SaveGameObject;
}

FString UHSaveSubsystem::GetLevelSlotName(const UWorld* World)
{
	return SaveConstants::LevelPrefix + UGameplayStatics::GetCurrentLevelName(World);
}
