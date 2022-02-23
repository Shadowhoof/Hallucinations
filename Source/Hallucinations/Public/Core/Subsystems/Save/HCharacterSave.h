// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HPersistentItem.h"
#include "Characters/HCharacterMetaData.h"
#include "GameFramework/SaveGame.h"
#include "HCharacterSave.generated.h"


/**
 *  Character save data which is saved to disk between sessions. Should not contain data which is only relevant for the current session.
 */
UCLASS(BlueprintType)
class HALLUCINATIONS_API UHPlayerCharacterSave : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ECharacterClass Class;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString Name;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FString> EquippedAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FPersistentEquippedItem> EquippedItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FPersistentInventoryItem> InventoryItems;
	
};



USTRUCT()
struct FPlayerCharacterSessionState
{
	GENERATED_BODY()

	UPROPERTY()
	float Health;

	UPROPERTY()
	float Mana;
	
	UPROPERTY()
	TMap<FString, float> AbilityCooldowns;
};

/**
 *	Save of player character state which is relevant only for non-persistent data of the current session (like health,
 *	remaining cooldowns, etc.) 
 */
UCLASS()
class HALLUCINATIONS_API UHPlayerCharacterSessionSave : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	FPlayerCharacterSessionState State;
	
};


/** Save containing names of all existing character saves. */
UCLASS()
class HALLUCINATIONS_API UHCharacterListSave : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	TArray<FString> CharacterNames;

};