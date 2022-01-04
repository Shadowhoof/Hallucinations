// Created by Dmitrii Korovin


#include "Core/HGameInstance.h"

UHInventoryItem* UHGameInstance::CreateItemById(int32 ItemId)
{
	if (!ItemMap.Contains(ItemId))
	{
		UE_LOG(LogDataTable, Error, TEXT("No item found for id %d"), ItemId);
		return nullptr;
	}

	const FName& RowName = ItemMap[ItemId];
	const FInventoryItem* ItemData = ItemTable->FindRow<FInventoryItem>(RowName, FString::FromInt(ItemId));
	return UHInventoryItem::CreateItem(ItemData);
}

void UHGameInstance::OnStart()
{
	Super::OnStart();
	
	if (ItemTable)
	{
		for (const auto& Entry : ItemTable->GetRowMap())
		{
			const FInventoryItem* ItemPtr = reinterpret_cast<FInventoryItem*>(Entry.Value);
			ItemMap.Add(ItemPtr->ItemId, Entry.Key);
		}
	}
	else
	{
		UE_LOG(LogDataTable, Error, TEXT("No item database specified for game instance"));
	}
}
