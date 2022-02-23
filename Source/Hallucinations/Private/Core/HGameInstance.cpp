// Created by Dmitrii Korovin


#include "Core/HGameInstance.h"

#include "Characters/HCharacterMetaData.h"
#include "Utils/HEnumTools.h"

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

FCharacterClassMetaData UHGameInstance::GetClassMetaData(ECharacterClass Class)
{
	if (!ClassMetaDataMap.Contains(Class))
	{
		UE_LOG(LogDataTable, Error, TEXT("No class metadata found for class %s"), *EnumAsString(Class));
		return FCharacterClassMetaData();
	}

	return ClassMetaDataMap[Class];
}

void UHGameInstance::OnStart()
{
	Super::OnStart();
	
	InitializeItemMap();
	InitializeClassMetaDataMap();
}

void UHGameInstance::InitializeItemMap()
{
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

void UHGameInstance::InitializeClassMetaDataMap()
{
	if (ClassMetaDataTable)
	{
		for (const auto& Entry : ClassMetaDataTable->GetRowMap())
		{
			const FCharacterClassMetaData* MetaDataPtr = reinterpret_cast<FCharacterClassMetaData*>(Entry.Value);
			ClassMetaDataMap.Add(MetaDataPtr->Class, *MetaDataPtr);
		}
	}
	else
	{
		UE_LOG(LogDataTable, Error, TEXT("No class metadata table specified for game instance"));
	}
}
