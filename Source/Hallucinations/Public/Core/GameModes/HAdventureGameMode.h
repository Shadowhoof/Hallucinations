#pragma once

#include "CoreMinimal.h"
#include "Core/GameModes/HGameMode.h"
#include "HAdventureGameMode.generated.h"

struct FPersistentActorState;

/**
 *  Game mode which is used in Adventure Mode (regular gameplay)
 */
UCLASS()
class HALLUCINATIONS_API AHAdventureGameMode : public AHGameMode
{
	GENERATED_BODY()

public:
	
	virtual void StartPlay() override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	virtual void StartToLeaveMap() override;
	
private:

	void CreateLevel();
	bool LoadLevel();

	void SaveLevelState();
	void RestoreSavedActor(const FPersistentActorState& State);
};
