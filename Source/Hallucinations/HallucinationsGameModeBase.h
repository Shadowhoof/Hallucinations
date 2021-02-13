// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HallucinationsGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API AHallucinationsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	
	virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;

};
