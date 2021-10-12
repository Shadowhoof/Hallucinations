// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/HLootableItem.h"

#include "HConstants.h"
#include "Characters/HPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/HEnumTools.h"
#include "Inventory/HInventoryItem.h"
#include "Core/HLogCategories.h"

AHLootableItem::AHLootableItem()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Click, ECR_Block);
}

void AHLootableItem::InteractWith(AHCharacter* Interactor)
{
	UHInventoryComponent* InventoryComponent = Cast<UHInventoryComponent>(Interactor->GetComponentByClass(UHInventoryComponent::StaticClass()));
	if (!InventoryComponent)
	{
		return;
	}

	bool bIsPickedUp = InventoryComponent->InsertItem(ItemData);
	Execute_OnHoverEnd(this);
	if (bIsPickedUp)
	{
		Destroy();
	}
}

float AHLootableItem::GetInteractionRange() const
{
	return InteractableConstants::DefaultInteractionRange;
}

FVector AHLootableItem::GetInteractableLocation() const
{
	return GetActorLocation();
}

void AHLootableItem::Initialize(UHInventoryItem* ProvidedData)
{
	ItemData = ProvidedData;
	StaticMeshComponent->SetStaticMesh(ItemData->GetData().Mesh);
}

AHLootableItem* AHLootableItem::SpawnItemFromData(const UObject* WorldContextObject, TSubclassOf<AHLootableItem> ItemClass, const FInventoryItem& Data, const FTransform& Transform)
{
	UHInventoryItem* Item = UHInventoryItem::CreateItem(Data);
	return SpawnItemFromObject(WorldContextObject, ItemClass, Item, Transform);
}

AHLootableItem* AHLootableItem::SpawnItemFromObject(const UObject* WorldContextObject,
	TSubclassOf<AHLootableItem> ItemClass, UHInventoryItem* Item, const FTransform& Transform)
{
	AActor* Actor = UGameplayStatics::BeginDeferredActorSpawnFromClass(WorldContextObject, ItemClass, Transform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	AHLootableItem* LootableItem = Cast<AHLootableItem>(Actor);
	LootableItem->Initialize(Item);
	UGameplayStatics::FinishSpawningActor(LootableItem, Transform);
	return LootableItem;
}

void AHLootableItem::BeginPlay()
{
	Super::BeginPlay();
}
