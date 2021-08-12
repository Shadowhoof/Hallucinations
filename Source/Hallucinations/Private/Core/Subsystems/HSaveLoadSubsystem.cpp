// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Subsystems/HSaveLoadSubsystem.h"

#include "Core/HSaveGame.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogSaveLoad);

const FString UHSaveLoadSubsystem::DefaultSlotName = "DefaultSlot";

void UHSaveLoadSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadGame();
}

void UHSaveLoadSubsystem::Deinitialize()
{
	SaveGame();
	
	Super::Deinitialize();
}

UHSaveGame* UHSaveLoadSubsystem::GetSaveData()
{
	return SaveData;
}

void UHSaveLoadSubsystem::SaveGame()
{
	ensure(SaveData);
	bool Result = UGameplayStatics::SaveGameToSlot(SaveData, DefaultSlotName, 0);
	if (Result)
	{
		UE_LOG(LogSaveLoad, Log, TEXT("Game saved successfully to slot %s!"), *DefaultSlotName);
	}
	else
	{
		UE_LOG(LogSaveLoad, Error, TEXT("Failed to save game to slot %s!"), *DefaultSlotName);
	}
}

void UHSaveLoadSubsystem::LoadGame()
{
	if (UGameplayStatics::DoesSaveGameExist(DefaultSlotName, 0))
	{
		SaveData = Cast<UHSaveGame>(UGameplayStatics::LoadGameFromSlot(DefaultSlotName, 0));
		if (!SaveData)
		{
			UE_LOG(LogSaveLoad, Warning, TEXT("Failed to load save file from slot %s, creating fresh save..."), *DefaultSlotName);
			CreateFreshSave();
		}

		UE_LOG(LogSaveLoad, Log, TEXT("Game successfully loaded from slot %s!"), *DefaultSlotName);
	}
	else
	{
		UE_LOG(LogSaveLoad, Log, TEXT("No save file found at slot %s, creating fresh save..."), *DefaultSlotName);
		CreateFreshSave();
	}
}

void UHSaveLoadSubsystem::CreateFreshSave()
{
	SaveData = Cast<UHSaveGame>(UGameplayStatics::CreateSaveGameObject(UHSaveGame::StaticClass()));
	UE_LOG(LogSaveLoad, Log, TEXT("Fresh save file successfully created!"));
}
