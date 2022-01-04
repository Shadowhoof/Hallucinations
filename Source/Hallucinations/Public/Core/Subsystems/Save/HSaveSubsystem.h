// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HSaveSubsystem.generated.h"

struct FPlayerCharacterSessionState;
class USaveGame;
class UHPlayerCharacterSave;
class UHPlayerCharacterSessionSave;
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

	UHPlayerCharacterSave* GetCharacterSaveData() const;

	/** Creates a level save for provided world */
	UHLevelSave* CreateLevelSave();

	/** Saves level state to disk */
	void SaveLevel(const UWorld* World, UHLevelSave* LevelSave);

	/** Loads level state of provided world */
	UHLevelSave* LoadLevel(const UWorld* World);

	/** Saves player character's session state */
	void SavePCSessionState(const FPlayerCharacterSessionState& PCState);

	/** Loads player character's session state */
	const UHPlayerCharacterSessionSave* LoadPCSessionState();

private:

	void SaveCharacter();
	void LoadCharacter();

	static USaveGame* CreateSave(TSubclassOf<USaveGame> SaveClass);
	static void Save(USaveGame* SaveGameObject, const FString& SlotName, int32 UserIndex);
	static USaveGame* Load(const FString& SlotName, int32 UserIndex);
	static USaveGame* LoadOrCreate(TSubclassOf<USaveGame> SaveClass, const FString& SlotName, int32 UserIndex);
	static void Delete(const FString& SlotName, int UserIndex);
	
	static FString GetLevelSlotName(const UWorld* World);
	static FString GetCharacterSlotName(const uint8 CharacterId);
	
	UPROPERTY()
	UHPlayerCharacterSave* CharacterSave;
	
	TSet<FString> VisitedLevelsSlotNames;
	
};