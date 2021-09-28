// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/HLootableItem.h"

#include "HConstants.h"
#include "Characters/HPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/HEnumTools.h"

DEFINE_LOG_CATEGORY(LogInventory);

AHLootableItem::AHLootableItem()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Click, ECR_Block);
}

void AHLootableItem::InteractWith(AHCharacter* Interactor)
{
	UE_LOG(LogInventory, Log, TEXT("Item %s of quality %s was picked up by actor %s"), *ItemData.Name.ToString(), *EnumToString(ItemData.Quality), *Interactor->GetName());
	Execute_OnHoverEnd(this);
	Destroy();
}

float AHLootableItem::GetInteractionRange() const
{
	return InteractableConstants::DefaultInteractionRange;
}

FVector AHLootableItem::GetInteractableLocation() const
{
	return GetActorLocation();
}

void AHLootableItem::Initialize(const FInventoryItem ProvidedData)
{
	ItemData = ProvidedData;
	StaticMeshComponent->SetStaticMesh(ItemData.Mesh);
}

AHLootableItem* AHLootableItem::SpawnItem(const UObject* WorldContextObject, TSubclassOf<AHLootableItem> ItemClass, const FInventoryItem& Data, const FTransform& Transform)
{
	AActor* Actor = UGameplayStatics::BeginDeferredActorSpawnFromClass(WorldContextObject, ItemClass, Transform, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	AHLootableItem* Item = Cast<AHLootableItem>(Actor);
	Item->Initialize(Data);
	UGameplayStatics::FinishSpawningActor(Item, Transform);
	return Item;
}

void AHLootableItem::BeginPlay()
{
	Super::BeginPlay();
}
