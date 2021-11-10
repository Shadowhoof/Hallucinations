// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/GameModes/HGameMode.h"
#include "HWaveGameMode.generated.h"

class AHCharacter;

UENUM()
enum class EWaveState
{
	Preparation,
	Spawning,
	WaveInProgress,
	GameOver
};

/**
 * Spawns a few waves of predetermined enemies while increasing their health and damage
 * with every subsequent wave. Game ends when all waves are defeated.
 */
UCLASS()
class HALLUCINATIONS_API AHWaveGameMode : public AHGameMode
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnStateChange();
	
private:

	UPROPERTY(VisibleInstanceOnly, Category = "Wave")
	EWaveState WaveState = EWaveState::Preparation;

	UPROPERTY(VisibleInstanceOnly, Category = "Wave")
	uint8 CurrentWave = 0;
	
	UPROPERTY(EditDefaultsOnly, Category = "Wave")
	uint8 TotalWaveCount = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Enemies")
	TArray<TSubclassOf<AHCharacter>> EnemyTypes;

	UPROPERTY(EditDefaultsOnly, Category = "Enemies")
	uint8 EnemiesPerWave = 2;
	
	UPROPERTY(EditDefaultsOnly, Category = "Enemies")
	float DamageIncreasePerWave = 5.0;

	UPROPERTY(EditDefaultsOnly, Category = "Enemies")
	float HealthIncreasePerWave = 20.0;

	void SetWaveState(EWaveState NewState);
	
	/* Time between waves */
	UPROPERTY(EditDefaultsOnly, Category = "Timers")
	float PreparationDuration = 5.0;
	
	FTimerHandle PreparationTimerHandle;

	TArray<AHCharacter*> SpawnedActors;
	
	void PrepareForNextWave();

	void EndWave();
	
	void SpawnEnemies();

	UFUNCTION()
	void OnEnemyDeath(AHCharacter* Victim, AActor* Killer);

public:

	UFUNCTION(BlueprintCallable)
	EWaveState GetWaveState() const;

	UFUNCTION(BlueprintCallable)
	uint8 GetCurrentWave() const;

};
