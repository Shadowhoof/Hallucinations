#include "Core/GameModes/HAdventureGameMode.h"

#include "EngineUtils.h"
#include "Constants/HConstants.h"
#include "Characters/HNonPlayerCharacter.h"
#include "Characters/HPlayerCharacter.h"
#include "Core/Subsystems/Save/HCharacterSave.h"
#include "Core/Subsystems/Save/HSaveSubsystem.h"
#include "Core/Subsystems/Save/HLevelSave.h"
#include "Engine/PlayerStartPIE.h"
#include "Level/HEnemySpawnPoint.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/HUtils.h"

DEFINE_LOG_CATEGORY(LogSpawn);

void AHAdventureGameMode::StartPlay()
{
	Super::StartPlay();

	SaveSubsystem = GetGameInstance()->GetSubsystem<UHSaveSubsystem>();
	ensure(SaveSubsystem);

	RestorePlayerCharacter();
	
	if (!LoadLevel())
	{
		CreateLevel();
	}
}

AActor* AHAdventureGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	if (AActor* StartPoint = Super::ChoosePlayerStart_Implementation(Player); StartPoint->IsA<APlayerStartPIE>())
	{
		return StartPoint;
	}

	FString PlayerStartTag = UGameplayStatics::ParseOption(OptionsString, HallucinationsConstants::PlayerStartTag);
	if (PlayerStartTag.IsEmpty())
	{
		UE_LOG(LogGameMode, Log, TEXT("Options string has no PlayerStartTag, using default value"));
		PlayerStartTag = HallucinationsConstants::DefaultPlayerStartTag;
	}

	for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		if (APlayerStart* PlayerStart = *It; PlayerStart->PlayerStartTag == FName(PlayerStartTag))
		{
			UE_LOG(LogGameMode, Log, TEXT("Found PlayerStart with tag %s"), *PlayerStartTag);
			return PlayerStart;
		}
	}

	UE_LOG(LogGameMode, Log, TEXT("Unable to find PlayerStart with tag %s, spawning at random location"), *PlayerStartTag);
	return Super::ChoosePlayerStart_Implementation(Player);
}

void AHAdventureGameMode::StartToLeaveMap()
{
	Super::StartToLeaveMap();

	SaveLevelState();
	SavePlayerCharacter();
	UE_LOG(LogLevel, Verbose, TEXT("Leaving map %s"), *UGameplayStatics::GetCurrentLevelName(this));
}

void AHAdventureGameMode::CreateLevel()
{
	FString LevelName = UGameplayStatics::GetCurrentLevelName(this);
	UE_LOG(LogLevel, Log, TEXT("No saved state for level %s found, randomizing..."), *LevelName);
	
	for (TActorIterator<AHEnemySpawnPoint> It(GetWorld()); It; ++It)
	{
		const AHEnemySpawnPoint* SpawnPoint = *It;
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

bool AHAdventureGameMode::LoadLevel()
{
	UHLevelSave* LevelSave = SaveSubsystem->LoadLevel(GetWorld());
	if (!LevelSave)
	{
		return false;
	}
	
	for (const auto& ActorData : LevelSave->GetActorData())
	{
		RestoreSavedActor(ActorData);
	}
	return true;
}

void AHAdventureGameMode::SaveLevelState()
{
	UHLevelSave* LevelSave = SaveSubsystem->CreateLevelSave();
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UHStatefulActorInterface::StaticClass(), Actors);
	for (const AActor* Actor : Actors)
	{
		const IHStatefulActorInterface* StatefulActor = Cast<IHStatefulActorInterface>(Actor);
		if (StatefulActor->ShouldBeSaved())
		{
			LevelSave->SaveActorState(StatefulActor->GetPersistentState());
		}
	}

	SaveSubsystem->SaveLevel(GetWorld(), LevelSave);
}

void AHAdventureGameMode::SavePlayerCharacter()
{
	AHPlayerCharacter* PlayerCharacter = Cast<AHPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	SaveSubsystem->SavePCSessionState(PlayerCharacter->GetSessionState());
}

void AHAdventureGameMode::RestoreSavedActor(const FPersistentActorState& State)
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	AActor* Actor = GetWorld()->SpawnActor(State.Class, &State.Transform, SpawnParams);

	IHStatefulActorInterface* StatefulActor = Cast<IHStatefulActorInterface>(Actor);
	if (StatefulActor)
	{
		StatefulActor->RestorePersistentState(State);
	}
}

void AHAdventureGameMode::RestorePlayerCharacter()
{
	const UHPlayerCharacterSessionSave* SessionSave = SaveSubsystem->LoadPCSessionState();
	if (SessionSave)
	{
		AHPlayerCharacter* PlayerCharacter = Cast<AHPlayerCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
		PlayerCharacter->RestoreSessionState(SessionSave->State);
	}
}
