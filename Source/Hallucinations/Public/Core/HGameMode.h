// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HGameMode.generated.h"

class AHLootableItem;
class UDataTable;
class UHSaveGame;
class AHCharacter;

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API AHGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	AHGameMode();

	void OnActorDeath(AActor* Victim, AActor* Killer);

	TSubclassOf<AHLootableItem> GetLootClass() const;
	
protected:
	
	virtual void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation) override;

	UPROPERTY(EditDefaultsOnly, Category = "Loot")
	UDataTable* LootTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Loot")
	TSubclassOf<AHLootableItem> LootClass;

	UFUNCTION(BlueprintCallable)
	void SpawnRandomItem(const FVector& Location);
	
private:

	void DropLoot(AHCharacter* Victim);
	
};
