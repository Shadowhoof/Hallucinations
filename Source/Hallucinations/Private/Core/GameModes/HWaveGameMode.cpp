// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/GameModes/HWaveGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/HEnemySpawnPoint.h"
#include "Characters/HCharacter.h"

void AHWaveGameMode::BeginPlay()
{
	ensure(WaveState == EWaveState::Preparation);
	UE_LOG(LogGameMode, Log, TEXT("Wave mode has started!"))
	PrepareForNextWave();
}

void AHWaveGameMode::SetWaveState(EWaveState NewState)
{
	if (WaveState != NewState)
	{
		const FString OldStateString = StaticEnum<EWaveState>()->GetValueAsString(WaveState);
		const FString NewStateString = StaticEnum<EWaveState>()->GetValueAsString(NewState);
		UE_LOG(LogGameMode, Log, TEXT("Wave state changed from %s to %s"), *OldStateString, *NewStateString);
		WaveState = NewState;
	}
}

void AHWaveGameMode::PrepareForNextWave()
{
	SetWaveState(EWaveState::Preparation);
	GetWorld()->GetTimerManager().SetTimer(PreparationTimerHandle, this, &AHWaveGameMode::SpawnEnemies, PreparationDuration);
}

void AHWaveGameMode::EndWave()
{
	CurrentWave += 1;
	if (CurrentWave < TotalWaveCount)
	{
		PrepareForNextWave();
	}
	else
	{
		SetWaveState(EWaveState::GameOver);
	}
}

void AHWaveGameMode::SpawnEnemies()
{
	TArray<AActor*> SpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AHEnemySpawnPoint::StaticClass(), SpawnPoints);
	if (SpawnPoints.Num() == 0)
	{
		WaveState = EWaveState::GameOver;
		UE_LOG(LogGameMode, Error, TEXT("No spawn points found!"));
		return;
	}

	if (EnemyTypes.Num() == 0 || EnemiesPerWave == 0)
	{
		WaveState = EWaveState::GameOver;
		UE_LOG(LogGameMode, Error, TEXT("Enemies could not be spawned! Enemy type count: %d, enemies per wave: %d"), EnemyTypes.Num(), EnemiesPerWave);
		return;
	}

	SetWaveState(EWaveState::Spawning);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	for (int i = 0; i < EnemiesPerWave; i++)
	{
		const int RandomEnemyIndex = FMath::RandRange(0, EnemyTypes.Num() - 1);
		TSubclassOf<AHCharacter> RandomEnemy = EnemyTypes[RandomEnemyIndex];
		
		const int RandomSpawnPointIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
		AHEnemySpawnPoint* SpawnPoint = Cast<AHEnemySpawnPoint>(SpawnPoints[RandomSpawnPointIndex]);
		FVector SpawnLocation = SpawnPoint->GetActorLocation() + FVector(0.f, 0.f, 300.f);
		FRotator SpawnRotation = SpawnPoint->GetActorRotation();
		
		AHCharacter* SpawnedActor = Cast<AHCharacter>(GetWorld()->SpawnActor(RandomEnemy, &SpawnLocation, &SpawnRotation, SpawnParams));
		SpawnedActor->DeathEvent.AddDynamic(this, &AHWaveGameMode::OnEnemyDeath);
		SpawnedActors.Add(SpawnedActor);
		
		UE_LOG(LogGameMode, Log, TEXT("Spawned %s at location %s"), *SpawnedActor->GetName(), *SpawnedActor->GetActorLocation().ToString());
	}

	SetWaveState(EWaveState::WaveInProgress);
}

void AHWaveGameMode::OnEnemyDeath(AHCharacter* Victim, AActor* Killer)
{
	SpawnedActors.Remove(Victim);
	if (SpawnedActors.Num() == 0)
	{
		EndWave();
	}
}

EWaveState AHWaveGameMode::GetWaveState() const
{
	return WaveState;
}

uint8 AHWaveGameMode::GetCurrentWave() const
{
	return CurrentWave;
}
