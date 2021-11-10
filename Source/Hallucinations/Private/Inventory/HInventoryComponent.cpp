// Fill out your copyright notice in the Description page of Project Settings.


#include "Inventory/HInventoryComponent.h"

#include "Core/GameModes/HGameMode.h"
#include "Core/HLogCategories.h"
#include "Inventory/HInventoryItem.h"
#include "Inventory/HLootableItem.h"
#include "Utils/HLogUtils.h"

DEFINE_LOG_CATEGORY(LogInventory);

UHInventoryComponent::UHInventoryComponent()
{
	Dimensions = { 10, 4 };
}

UHInventoryItem* UHInventoryComponent::GetItemAt(const FInventoryCell& TopLeftCell)
{
	if (!ValidateCell(TopLeftCell))
	{
		return nullptr;
	}

	return Grid[TopLeftCell.Row * Dimensions.Width + TopLeftCell.Column];
}

bool UHInventoryComponent::InsertItem(UHInventoryItem* Item)
{
	if (!Item)
	{
		UE_LOG(LogInventory, Error, TEXT("Tried to pick up null item"));
		return false;
	}
	
	FInventoryDimensions ItemDimensions = Item->GetData().Dimensions;
	for (int32 Column = 0; Column <= Dimensions.Width - ItemDimensions.Width; Column++)
	{
		for (int32 Row = 0; Row <= Dimensions.Height - ItemDimensions.Height; Row++)
		{
			FInventoryCell Cell{ Row, Column };
			if (InsertItemAt(Item, Cell))
			{
				return true;
			}
		}
	}

	UE_LOG(LogInventory, VeryVerbose, TEXT("Failed to insert item %s, no space available"), *Item->GetData().Name.ToString());
	return false;
}

bool UHInventoryComponent::InsertItemAt(UHInventoryItem* Item, const FInventoryCell& TopLeftCell)
{
	FInventoryDimensions ItemDimensions = Item->GetData().Dimensions;
	if (!CanInsertItemAt(ItemDimensions, TopLeftCell))
	{
		return false;
	}

	for (int32 Row = TopLeftCell.Row; Row < TopLeftCell.Row + ItemDimensions.Height; Row++)
	{
		for (int32 Column = TopLeftCell.Column; Column < TopLeftCell.Column + ItemDimensions.Width; Column++)
		{
			int32 CellIndex = Row * Dimensions.Width + Column;
			ensure(!Grid[CellIndex]);
			Grid[CellIndex] = Item;
		}
	}
	ItemMap.Add(Item, TopLeftCell);
	UE_LOG(LogInventory, VeryVerbose, TEXT("Inserted item %s at position (%d, %d)"), *Item->GetData().Name.ToString(), TopLeftCell.Row, TopLeftCell.Column);
	OnInventoryChanged.Broadcast(this);
	return true;
}

bool UHInventoryComponent::CanInsertItemAt(const FInventoryDimensions& ItemDimensions, const FInventoryCell& TopLeftCell) const
{
	if (!ValidateDimensionsAt(ItemDimensions, TopLeftCell))
	{
		return false;
	}

	for (int32 Row = TopLeftCell.Row; Row < TopLeftCell.Row + ItemDimensions.Height; Row++)
	{
		for (int32 Column = TopLeftCell.Column; Column < TopLeftCell.Column + ItemDimensions.Width; Column++)
		{
			int32 GridIndex = Row * Dimensions.Width + Column;
			if (Grid[GridIndex])
			{
				return false;
			}
		}
	}

	return true;
}

void UHInventoryComponent::RemoveItem(UHInventoryItem* Item)
{
	FInventoryCell* CellPtr = ItemMap.Find(Item);
	ensure(CellPtr);
	
	FInventoryCell Cell = *CellPtr;
	const FInventoryDimensions& ItemDimensions = Item->GetData().Dimensions;
	for (int32 Row = Cell.Row; Row < Cell.Row + ItemDimensions.Height; Row++)
	{
		for (int32 Column = Cell.Column; Column < Cell.Column + ItemDimensions.Width; Column++)
		{
			int32 Index = Row * Dimensions.Width + Column;
			ensure(Grid[Index] == Item);
			Grid[Index] = nullptr;
		}
	}
	ItemMap.Remove(Item);
	UE_LOG(LogInventory, VeryVerbose, TEXT("Item %s removed from inventory"), *Item->GetData().Name.ToString());
	OnInventoryChanged.Broadcast(this);
}

void UHInventoryComponent::DropItem(UHInventoryItem* Item)
{
	AHGameMode* GameMode = Cast<AHGameMode>(GetWorld()->GetAuthGameMode());
	TSubclassOf<AHLootableItem> ItemClass = GameMode->GetLootClass();
	AHLootableItem::SpawnItemFromObject(this, ItemClass, Item, GetOwner()->GetActorTransform());
}

void UHInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Grid.Init(nullptr, Dimensions.Width * Dimensions.Height);
}

bool UHInventoryComponent::ValidateCell(const FInventoryCell& Cell) const
{
	return 0 <= Cell.Row && Cell.Row < Dimensions.Height && 0 <= Cell.Column && Cell.Column < Dimensions.Width;
}

bool UHInventoryComponent::ValidateDimensionsAt(const FInventoryDimensions& ItemDimensions, const FInventoryCell& TopLeftCell) const
{
	const int32 BottomRow = TopLeftCell.Row + ItemDimensions.Height - 1;
	const int32 RightColumn = TopLeftCell.Column + ItemDimensions.Width - 1;
	FInventoryCell BottomRightCell{ BottomRow, RightColumn };
	return ValidateCell(TopLeftCell) && ValidateCell(BottomRightCell);
}
