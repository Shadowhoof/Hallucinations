#pragma once
#include "Inventory/HEquipmentType.h"
#include "Inventory/HInventoryItem.h"

#include "HPersistentItem.generated.h"


/**
 *	Equipped item information that will be saved to and loaded from disk
 */
USTRUCT(BlueprintType)
struct FPersistentEquippedItem
{
	GENERATED_BODY()

	FPersistentEquippedItem()
		: Slot(EEquipmentSlot::None), ItemId(-1) {}
	
	FPersistentEquippedItem(const EEquipmentSlot InSlot, const FInventoryItem& ItemData)
		: Slot(InSlot), ItemId(ItemData.ItemId)
	{
	}
	
	UPROPERTY()
	EEquipmentSlot Slot;

	UPROPERTY()
	int32 ItemId;
};

/**
 *	Inventory item information that will be saved to and loaded from disk
 */
USTRUCT(BlueprintType)
struct FPersistentInventoryItem
{
	GENERATED_BODY()

	FPersistentInventoryItem()
		: Position({-1, -1}), ItemId(-1) {}
	
	FPersistentInventoryItem(const FInventoryCell InPosition, const FInventoryItem& ItemData)
		: Position(InPosition), ItemId(ItemData.ItemId)
	{
	}

	UPROPERTY()
	FInventoryCell Position;
	
	UPROPERTY()
	int32 ItemId;
};
