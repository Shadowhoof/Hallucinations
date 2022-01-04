// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "Inventory/HInventoryItem.h"
#include "HGameInstance.generated.h"

class UDataTable;

/**
 *	Custom game instance class for Hallucinations.
 */
UCLASS()
class HALLUCINATIONS_API UHGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	/**
	 *	Retrieves item data from item table by itemId and creates a new item from that data.
	 *	Can be nullptr if no item is found for provided id
	 *	@param ItemId unique item identifier
	 */
	UHInventoryItem* CreateItemById(int32 ItemId);
	
protected:

	virtual void OnStart() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	UDataTable* ItemTable;

private:
	
	TMap<int32, FName> ItemMap;
	
};
