// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API UHSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UHSaveGame();

	UPROPERTY()
	TArray<FString> EquippedAbilities;

};
