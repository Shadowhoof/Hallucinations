#include "Inventory/HInventoryItem.h"

UHInventoryItem::UHInventoryItem()
{
}

const FInventoryItem& UHInventoryItem::GetData() const
{
	return Data;
}

UHInventoryItem* UHInventoryItem::CreateItem(const FInventoryItem& Data)
{
	UHInventoryItem* Item = NewObject<UHInventoryItem>();
	Item->Data = Data;
	return Item;
}

UHInventoryItem* UHInventoryItem::CreateItem(const FInventoryItem* DataPtr)
{
	return DataPtr ? CreateItem(*DataPtr) : nullptr;
}
