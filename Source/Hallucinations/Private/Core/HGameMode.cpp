// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/HGameMode.h"

#include "NavigationSystem.h"
#include "Characters/HPlayerCharacter.h"
#include "Components/HHealthComponent.h"
#include "Controllers/HPlayerController.h"
#include "Engine/DataTable.h"
#include "Inventory/HLootableItem.h"

AHGameMode::AHGameMode()
{
	DefaultPawnClass = AHPlayerCharacter::StaticClass();
	PlayerControllerClass = AHPlayerController::StaticClass();
}

void AHGameMode::OnActorDeath(AActor* Victim, AActor* Killer)
{
	AHCharacter* VictimCharacter = Cast<AHCharacter>(Victim);
	if (VictimCharacter && VictimCharacter->GetHealthComponent()->GetTeam() == ETeam::BadGuys)
	{
		DropLoot(VictimCharacter);
	}
}

void AHGameMode::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	Super::FinishRestartPlayer(NewPlayer, StartRotation);

	if (NewPlayer->GetPawn() != nullptr) 
	{
		AHPlayerController* PlayerController = Cast<AHPlayerController>(NewPlayer);
		if (PlayerController) 
		{
			PlayerController->OnPlayerRestart();
		}
	}
}

void AHGameMode::DropLoot(AHCharacter* Victim)
{
	if (!LootTable || !LootClass)
	{
		return;
	}

	TMap<FName, uint8*> RowMap = LootTable->GetRowMap();
	if (RowMap.Num() == 0)
	{
		return;
	}
	
	TArray<FName> KeyArray;
	RowMap.GetKeys(KeyArray);
	uint32 RandomIndex = FMath::RandRange(0, KeyArray.Num());
	FInventoryItem* ItemData = reinterpret_cast<FInventoryItem*>(RowMap[KeyArray[RandomIndex]]);
	if (ItemData)
	{
		UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		FNavLocation RandomNavLocation;
		NavSystem->GetRandomPointInNavigableRadius(Victim->GetActorLocation(), LootConstants::DropRadius, RandomNavLocation);
		AHLootableItem::SpawnItem(this, LootClass, *ItemData, FTransform(RandomNavLocation.Location));
	}
}
