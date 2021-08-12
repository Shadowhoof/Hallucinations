// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HGameMode.generated.h"

class UHSaveGame;
/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API AHGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AHGameMode();

protected:
	
	virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;

};
