// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Subsystems/Save/HSaveSubsystem.h"

#include "Core/Subsystems/Save/HCharacterSave.h"
#include "Core/Subsystems/Save/HLevelSave.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogSave);

namespace SaveConstants
{
	const FString LevelPrefix = "Level:";
	const FString PCSessionSlotName = "CharacterSession";
	const FString CharacterPrefix = "Character:";
	const FString CharacterListSlotName = "CharacterList";
}


void UHSaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	LoadCharacterList();
}

void UHSaveSubsystem::Deinitialize()
{
	SaveCharacter();
	SaveCharacterList();

	// level data should not be saved between sessions
	for (const auto& SlotName : VisitedLevelsSlotNames)
	{
		Delete(SlotName, 0);
	}

	// neither should player character session data
	Delete(SaveConstants::PCSessionSlotName, 0);
	
	Super::Deinitialize();
}

const TArray<UHPlayerCharacterSave*>& UHSaveSubsystem::GetAllCharacterSaves() const
{
	TArray<FString> NotExistingNames;
	if (AllCharacterSaves.IsEmpty() && !CharacterListSave->CharacterNames.IsEmpty())
	{
		for (const FString& CharacterName : CharacterListSave->CharacterNames)
		{
			const FString CharacterSlotName = GetCharacterSlotName(CharacterName);
			UHPlayerCharacterSave* LoadedSave = Cast<UHPlayerCharacterSave>(Load(CharacterSlotName, 0));
			if (LoadedSave)
			{
				AllCharacterSaves.Add(LoadedSave);
			}
			else
			{
				NotExistingNames.Add(CharacterName);
			}
		}
	}

	if (!NotExistingNames.IsEmpty())
	{
		for (const FString& CharacterName : NotExistingNames)
		{
			UE_LOG(LogSave, Warning, TEXT("Save with name %s does not exist, deleting it from the character list"), *CharacterName);
			CharacterListSave->CharacterNames.Remove(CharacterName);
		}
	}
	
	return AllCharacterSaves;
}

UHPlayerCharacterSave* UHSaveSubsystem::GetCharacterSaveData() const
{
	return CurrentCharacterSave;
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

void UHSaveSubsystem::SavePCSessionState(const FPlayerCharacterSessionState& PCState)
{
	UHPlayerCharacterSessionSave* SessionSave = Cast<UHPlayerCharacterSessionSave>(CreateSave(UHPlayerCharacterSessionSave::StaticClass()));
	SessionSave->State = PCState;
	Save(SessionSave, SaveConstants::PCSessionSlotName, 0);
}

const UHPlayerCharacterSessionSave* UHSaveSubsystem::LoadPCSessionState()
{
	return Cast<UHPlayerCharacterSessionSave>(Load(SaveConstants::PCSessionSlotName, 0));
}

bool UHSaveSubsystem::LoadCharacterSave(const FString& CharacterName)
{
	const FString SlotName = GetCharacterSlotName(CharacterName);
	CurrentCharacterSave = Cast<UHPlayerCharacterSave>(Load(SlotName, 0));
	return CurrentCharacterSave != nullptr;
}

void UHSaveSubsystem::DeleteCharacterSave(const FString& CharacterName)
{
	const FString SlotName = GetCharacterSlotName(CharacterName);
	Delete(SlotName, 0);
	CharacterListSave->CharacterNames.Remove(CharacterName);
	
	AllCharacterSaves.RemoveAll([&CharacterName](const UHPlayerCharacterSave* Save)
	{
		return Save->Name == CharacterName;
	});
}

void UHSaveSubsystem::LoadCharacterList()
{
	USaveGame* NewSave = LoadOrCreate(UHCharacterListSave::StaticClass(), SaveConstants::CharacterListSlotName, 0);
	CharacterListSave = Cast<UHCharacterListSave>(NewSave);
}

void UHSaveSubsystem::SaveCharacterList()
{
	Save(CharacterListSave, SaveConstants::CharacterListSlotName, 0);
}

void UHSaveSubsystem::SaveCharacter()
{
	if (!CurrentCharacterSave)
	{
		return;
	}
	
	const FString SlotName = GetCharacterSlotName(CurrentCharacterSave->Name);
	Save(CurrentCharacterSave, SlotName, 0);
}

bool UHSaveSubsystem::CreateCharacterSave(const FString& CharacterName, ECharacterClass Class)
{
	if (CharacterListSave->CharacterNames.Contains(CharacterName))
	{
		// character with that name already exists
		return false;
	}

	UHPlayerCharacterSave* NewCharacterSave = Cast<UHPlayerCharacterSave>(CreateSave(UHPlayerCharacterSave::StaticClass()));
	NewCharacterSave->Class = Class;
	NewCharacterSave->Name = CharacterName;
	// FIXME change character slot name
	Save(NewCharacterSave, GetCharacterSlotName(CharacterName), 0);
	
	CharacterListSave->CharacterNames.Add(CharacterName);
	AllCharacterSaves.Add(NewCharacterSave);

	// FIXME creation should not automatically start a game
	CurrentCharacterSave = NewCharacterSave;
	
	return true;
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

void UHSaveSubsystem::Delete(const FString& SlotName, int UserIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		UE_LOG(LogSave, VeryVerbose, TEXT("Deleting save %s"), *SlotName);
		UGameplayStatics::DeleteGameInSlot(SlotName, UserIndex);
	}
}

FString UHSaveSubsystem::GetLevelSlotName(const UWorld* World)
{
	return SaveConstants::LevelPrefix + UGameplayStatics::GetCurrentLevelName(World);
}

FString UHSaveSubsystem::GetCharacterSlotName(const uint8 CharacterId)
{
	return SaveConstants::CharacterPrefix + FString::FromInt(CharacterId);
}

FString UHSaveSubsystem::GetCharacterSlotName(const FString& CharacterName)
{
	return SaveConstants::CharacterPrefix + CharacterName;
}
