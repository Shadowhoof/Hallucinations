// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HSaveSubsystem.generated.h"

class USaveGame;
class UHSaveGame;
class UHLevelSave;

DECLARE_LOG_CATEGORY_EXTERN(LogSave, Log, All);

/**
 * Subsystem that handles saving and loading of game data
 */
UCLASS()
class HALLUCINATIONS_API UHSaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UHSaveGame* GetSaveData() const;

	/** Creates a level save for provided world */
	UHLevelSave* CreateLevelSave();

	/** Saves level state to disk */
	void SaveLevel(const UWorld* World, UHLevelSave* LevelSave);

	/** Loads level state of provided world */
	UHLevelSave* LoadLevel(const UWorld* World);

	/** Deletes all level saves which were saved during this playthrough */
	void DeleteLevelSaves();
	
private:

	void SaveGame();
	void LoadGame();

	static USaveGame* CreateSave(TSubclassOf<USaveGame> SaveClass);
	static void Save(USaveGame* SaveGameObject, const FString& SlotName, int32 UserIndex);
	static USaveGame* Load(const FString& SlotName, int32 UserIndex);
	static USaveGame* LoadOrCreate(TSubclassOf<USaveGame> SaveClass, const FString& SlotName, int32 UserIndex);

	static FString GetLevelSlotName(const UWorld* World);
	
	UPROPERTY()
	UHSaveGame* SaveData;
	
	static const FString DefaultSlotName;

	TSet<FString> VisitedLevelsSlotNames;
	
};