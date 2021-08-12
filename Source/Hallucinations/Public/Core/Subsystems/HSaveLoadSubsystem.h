// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HSaveLoadSubsystem.generated.h"

class UHSaveGame;

DECLARE_LOG_CATEGORY_EXTERN(LogSaveLoad, Log, All);

/**
 * Subsystem that handles saving and loading of game data
 */
UCLASS()
class HALLUCINATIONS_API UHSaveLoadSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UHSaveGame* GetSaveData();
	
private:

	void SaveGame();
	void LoadGame();

	void CreateFreshSave();

	UPROPERTY()
	UHSaveGame* SaveData;
	
	static const FString DefaultSlotName;
};
