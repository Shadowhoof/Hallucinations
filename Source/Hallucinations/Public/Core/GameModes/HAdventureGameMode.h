#pragma once

#include "CoreMinimal.h"
#include "Core/GameModes/HGameMode.h"
#include "HAdventureGameMode.generated.h"

/**
 *  Game mode which is used in Adventure Mode (regular gameplay)
 */
UCLASS()
class HALLUCINATIONS_API AHAdventureGameMode : public AHGameMode
{
	GENERATED_BODY()

	virtual void StartPlay() override;

	virtual AActor* FindPlayerStart_Implementation(AController* Player, const FString& IncomingName) override;
	
};
