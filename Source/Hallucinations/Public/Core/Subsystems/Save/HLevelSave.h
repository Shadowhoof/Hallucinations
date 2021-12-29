// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "HLevelSave.generated.h"


class AHNonPlayerCharacter;


USTRUCT()
struct FPersistentActorState
{
	GENERATED_BODY()

	UPROPERTY()
	TSubclassOf<AHNonPlayerCharacter> Class;

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	float Health;
};

/**
 * Persistent level state. This state will be loaded and restored when player enters a previously visited level.   
 */
UCLASS()
class HALLUCINATIONS_API UHLevelSave : public USaveGame
{
	GENERATED_BODY()

public:
	
	void SaveActorState(FPersistentActorState State);
	
	const TArray<FPersistentActorState>& GetActorData();

private:
	
	UPROPERTY()
	TArray<FPersistentActorState> Actors;
	
};
