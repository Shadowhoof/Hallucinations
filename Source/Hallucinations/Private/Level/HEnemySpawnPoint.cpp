// Fill out your copyright notice in the Description page of Project Settings.


#include "Level/HEnemySpawnPoint.h"
#include "Components/ArrowComponent.h"
#include "Characters/HNonPlayerCharacter.h"
#include "Utils/HUtils.h"

AHEnemySpawnPoint::AHEnemySpawnPoint()
{
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	RootComponent = ArrowComponent;
}

TSubclassOf<AHNonPlayerCharacter> AHEnemySpawnPoint::GetRandomEnemyType() const
{
	if (EnemyTypes.IsEmpty())
	{
		return nullptr;
	}
	
	return UHUtils::GetRandomArrayElement(EnemyTypes);
}

void AHEnemySpawnPoint::GetSpawnableEnemyTypes(TArray<TSubclassOf<AHNonPlayerCharacter>>& OutTypes)
{
	OutTypes.Append(EnemyTypes);
}

int32 AHEnemySpawnPoint::GetRandomEnemyCount() const
{
	return FMath::RandRange(MinEnemyCount, MaxEnemyCount);
}

int32 AHEnemySpawnPoint::GetMinEnemyCount() const
{
	return MinEnemyCount;
}

int32 AHEnemySpawnPoint::GetMaxEnemyCount() const
{
	return MaxEnemyCount;
}

bool AHEnemySpawnPoint::CanSpawnElites() const
{
	return bCanSpawnElites;
}

float AHEnemySpawnPoint::GetSpawnRadius() const
{
	return SpawnRadius;
}
