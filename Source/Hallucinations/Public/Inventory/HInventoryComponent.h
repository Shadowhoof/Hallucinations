// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Delegates/Delegate.h"

#include "HInventoryComponent.generated.h"

class UHInventoryItem;
class UHInventoryComponent;


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInventoryChangedDelegate, UHInventoryComponent*, InventoryComponent);


USTRUCT(BlueprintType)
struct FInventoryDimensions
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1"))
	int32 Width;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "1"))
	int32 Height;

};


USTRUCT(BlueprintType)
struct FInventoryCell
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0"))
	int32 Row;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0"))
	int32 Column;
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HALLUCINATIONS_API UHInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UHInventoryComponent();

	UFUNCTION(BlueprintCallable)
	UHInventoryItem* GetItemAt(const FInventoryCell& Cell);

	UFUNCTION(BlueprintCallable)
	bool InsertItem(UHInventoryItem* Item);

	UFUNCTION(BlueprintCallable)
	bool InsertItemAt(UHInventoryItem* Item, const FInventoryCell& TopLeftCell);

	UFUNCTION(BlueprintCallable)
	bool CanInsertItemAt(const FInventoryDimensions& ItemDimensions, const FInventoryCell& TopLeftCell) const;

	/** Causes the item to be removed from the inventory, destroyed and to be gone forever. It does not drop on the ground. */
	UFUNCTION(BlueprintCallable)
	void RemoveItem(UHInventoryItem* Item);

	/** Causes the item to be removed from the inventory and be dropped on the ground at the player's feet. */
	UFUNCTION(BlueprintCallable)
	void DropItem(UHInventoryItem* Item);
	
	UPROPERTY(BlueprintAssignable)
	FInventoryChangedDelegate OnInventoryChanged;
	
protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Layout")
	FInventoryDimensions Dimensions;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	TMap<UHInventoryItem*, FInventoryCell> ItemMap;
	
private:
	
	TArray<UHInventoryItem*> Grid;

	bool ValidateCell(const FInventoryCell& Cell) const;

	bool ValidateDimensionsAt(const FInventoryDimensions& ItemDimensions, const FInventoryCell& TopLeftCell) const;
	
};
