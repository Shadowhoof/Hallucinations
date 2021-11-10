#include "Core/GameModes/HAdventureGameMode.h"
#include "HConstants.h"
#include "Characters/HNonPlayerCharacter.h"
#include "Actors/HEnemySpawnPoint.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utils/HUtils.h"

DEFINE_LOG_CATEGORY(LogSpawn);

void AHAdventureGameMode::StartPlay()
{
	Super::StartPlay();

	TArray<AActor*> EnemySpawnPoints;
	UGameplayStatics::GetAllActorsOfClass(this, AHEnemySpawnPoint::StaticClass(), EnemySpawnPoints);
	for (AActor* Actor : EnemySpawnPoints)
	{
		AHEnemySpawnPoint* SpawnPoint = Cast<AHEnemySpawnPoint>(Actor);
		int32 EnemyCount = SpawnPoint->GetRandomEnemyCount();
		if (EnemyCount <= 0)
		{
			UE_LOG(LogSpawn, Warning, TEXT("Couldn't spawn enemies at point %s because enemy count was 0"), *SpawnPoint->GetName());
			continue;
		}

		TSubclassOf<AHNonPlayerCharacter> EnemyType = SpawnPoint->GetRandomEnemyType();
		if (!EnemyType)
		{
			UE_LOG(LogSpawn, Warning, TEXT("Couldn't spawn enemies at point %s because no enemy type is specified for that point"), *SpawnPoint->GetName());
			continue;
		}
		
		UE_LOG(LogSpawn, Log, TEXT("Spawning %d enemies of type %s"), EnemyCount, *EnemyType->GetName())
		float SpawnRadius = SpawnPoint->GetSpawnRadius();
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		for (int32 i = 0; i < EnemyCount; ++i)
		{
			FVector SpawnLocation;
			UHUtils::GetRandomPointInNavigableRadius(this, SpawnPoint->GetActorLocation(), SpawnRadius, SpawnLocation);
			FRotator SpawnRotation(0.0, FMath::FRand() * 360.f, 0.0);
			GetWorld()->SpawnActor(EnemyType, &SpawnLocation, &SpawnRotation, SpawnParams);
		}
	}
}

AActor* AHAdventureGameMode::FindPlayerStart_Implementation(AController* Player, const FString& IncomingName)
{
	FString PlayerStartTag = UGameplayStatics::ParseOption(OptionsString, HallucinationsConstants::LevelExitTag);
	if (PlayerStartTag.IsEmpty())
	{
		UE_LOG(LogGameMode, Log, TEXT("Options string has no ExitTag, using default value"));
		PlayerStartTag = HallucinationsConstants::DefaultPlayerStartTag;
	}

	TArray<AActor*> PlayerStartArray;
	UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStartArray);
	for (AActor* Actor : PlayerStartArray)
	{
		APlayerStart* PlayerStart = Cast<APlayerStart>(Actor);
		if (PlayerStart->PlayerStartTag == FName(PlayerStartTag))
		{
			UE_LOG(LogGameMode, Log, TEXT("Found PlayerStart with tag %s"), *PlayerStartTag);
			return PlayerStart;
		}
	}

	UE_LOG(LogGameMode, Log, TEXT("Unable to find PlayerStart with tag %s, spawning at random location"), *PlayerStartTag);
	return Super::FindPlayerStart_Implementation(Player, IncomingName);
}
