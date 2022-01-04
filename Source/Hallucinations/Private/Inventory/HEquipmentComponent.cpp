// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/HEquipmentComponent.h"

#include "Core/HGameInstance.h"
#include "Core/GameModes/HGameMode.h"
#include "Core/Subsystems/Save/HCharacterSave.h"
#include "Core/Subsystems/Save/HPersistentItem.h"
#include "Core/Subsystems/Save/HSaveSubsystem.h"
#include "Inventory/HInventoryItem.h"
#include "Inventory/HLootableItem.h"
#include "Utils/HUtils.h"

namespace EquipmentConstants
{
	const float UnequipDropRadius = 100.f;
}

const TMap<EEquipmentType, const TArray<EEquipmentSlot>> UHEquipmentComponent::TypeToSlotMap = {
	{EEquipmentType::Head,		{EEquipmentSlot::Head}},
	{EEquipmentType::Chest,		{EEquipmentSlot::Chest}},
	{EEquipmentType::Waist,		{EEquipmentSlot::Waist}},
	{EEquipmentType::Hands,		{EEquipmentSlot::Hands}},
	{EEquipmentType::Feet,		{EEquipmentSlot::Feet}},
	{EEquipmentType::Ring,		{EEquipmentSlot::Ring1, EEquipmentSlot::Ring2}},
	{EEquipmentType::Amulet,	{EEquipmentSlot::Amulet}},
	{EEquipmentType::MainHand,	{EEquipmentSlot::MainHandWeapon}},
	{EEquipmentType::OffHand,	{EEquipmentSlot::OffHandWeapon}},
	{EEquipmentType::AnyHand,	{EEquipmentSlot::MainHandWeapon, EEquipmentSlot::OffHandWeapon}}
};

UHEquipmentComponent::UHEquipmentComponent()
{
	for (EEquipmentSlot Slot : TEnumRange<EEquipmentSlot>())
	{
		ItemMap.Add(Slot, nullptr);
	}
}

UHInventoryItem* UHEquipmentComponent::EquipItem(UHInventoryItem* Item, EEquipmentSlot Slot)
{
	if (!CanEquipItemInSlot(Item, Slot))
	{
		return nullptr;
	}
	
	UHInventoryItem* UnequippedItem = ItemMap[Slot];
	if (UnequippedItem)
	{
		ItemArray.Remove(UnequippedItem);
		OnItemUnequipped.Broadcast(UnequippedItem);
	}

	ItemMap[Slot] = Item;
	if (Item)
	{
		ItemArray.Add(Item);
		OnItemEquipped.Broadcast(Item);
	}

	return UnequippedItem;
}

void UHEquipmentComponent::ReplaceItem(UHInventoryItem* Item, EEquipmentSlot Slot)
{
	UHInventoryItem* UnequippedItem = EquipItem(Item, Slot);
	if (!UnequippedItem)
	{
		return;
	}
	
	UHInventoryComponent* InventoryComponent = Cast<UHInventoryComponent>(GetOwner()->GetComponentByClass(UHInventoryComponent::StaticClass()));
	bool bIsMovedToInventory = false;
	if (InventoryComponent)
	{
		bIsMovedToInventory = InventoryComponent->InsertItem(UnequippedItem);
	}

	if (!bIsMovedToInventory)
	{
		FVector DropLocation;
		UHUtils::GetRandomPointInNavigableRadius(this, GetOwner()->GetActorLocation(), EquipmentConstants::UnequipDropRadius, DropLocation);
		AHGameMode* GameMode = Cast<AHGameMode>(GetWorld()->GetAuthGameMode());
		AHLootableItem::SpawnItemFromObject(this, GameMode->GetLootClass(), UnequippedItem, FTransform(DropLocation));
	}
}

UHInventoryItem* UHEquipmentComponent::UnequipItem(EEquipmentSlot Slot)
{
	return EquipItem(nullptr, Slot);
}

void UHEquipmentComponent::GetSlotForType(EEquipmentType Type, TArray<EEquipmentSlot>& OutSlotArray) const
{
	static const TArray<EEquipmentSlot> EmptyArray;
	const TArray<EEquipmentSlot>* SlotArrayPtr = TypeToSlotMap.Find(Type);
	OutSlotArray = SlotArrayPtr ? *SlotArrayPtr : EmptyArray;
}

bool UHEquipmentComponent::CanEquipItemInSlot(UHInventoryItem* Item, EEquipmentSlot Slot) const
{
	if (!Item)
	{
		return true;
	}

	EEquipmentType ItemType = Item->GetData().EquipmentType;
	const TArray<EEquipmentSlot>* SlotArrayPtr = TypeToSlotMap.Find(ItemType);
	if (!SlotArrayPtr)
	{
		return false;
	}

	const TArray<EEquipmentSlot> SlotArray = *SlotArrayPtr;
	return SlotArray.Contains(Slot);
}

const TArray<const UHInventoryItem*>& UHEquipmentComponent::GetEquippedItems() const
{
	return ItemArray;
}

UHInventoryItem* UHEquipmentComponent::GetItemInSlot(EEquipmentSlot Slot) const
{
	return ItemMap[Slot];
}

void UHEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (EEquipmentSlot Slot : TEnumRange<EEquipmentSlot>())
	{
		ItemMap.Add(Slot, nullptr);
	}

	LoadPersistentData();
}

void UHEquipmentComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SavePersistentData();
}

UHPlayerCharacterSave* UHEquipmentComponent::GetCharacterSave()
{
	UHGameInstance* GameInstance = Cast<UHGameInstance>(GetWorld()->GetGameInstance());
	UHSaveSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UHSaveSubsystem>();
	return SaveSubsystem->GetCharacterSaveData(); 
}

void UHEquipmentComponent::SavePersistentData()
{
	UHPlayerCharacterSave* Save = GetCharacterSave();
	Save->EquippedItems.Empty();
	for (const auto& Entry : ItemMap)
	{
		if (const UHInventoryItem* Item = Entry.Value)
		{
			Save->EquippedItems.Add(FPersistentEquippedItem(Entry.Key, Item->GetData()));
		}
	}
}

void UHEquipmentComponent::LoadPersistentData()
{
	UHGameInstance* GameInstance = Cast<UHGameInstance>(GetWorld()->GetGameInstance());
	UHPlayerCharacterSave* Save = GetCharacterSave();
	for (const auto& Item : Save->EquippedItems)
	{
		EquipItem(GameInstance->CreateItemById(Item.ItemId), Item.Slot);
	}
}

