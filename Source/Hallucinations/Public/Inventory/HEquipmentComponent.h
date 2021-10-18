// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HEquipmentType.h"
#include "HEquipmentComponent.generated.h"


class UHInventoryItem;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HALLUCINATIONS_API UHEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UHEquipmentComponent();

	UFUNCTION(BlueprintCallable, Category = "Actions")
	UHInventoryItem* EquipItem(UHInventoryItem* Item, EEquipmentSlot Slot);

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void ReplaceItem(UHInventoryItem* Item, EEquipmentSlot Slot);
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	UHInventoryItem* UnequipItem(EEquipmentSlot Slot);

	UFUNCTION(BlueprintPure, Category = "Utility")
	void GetSlotForType(EEquipmentType Type, TArray<EEquipmentSlot>& OutSlotArray) const;

	UFUNCTION(BlueprintPure, Category = "Utility")
	bool CanEquipItemInSlot(UHInventoryItem* Item, EEquipmentSlot Slot) const;
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equipment")
	TMap<EEquipmentSlot, UHInventoryItem*> Items;

	virtual void BeginPlay() override;

private:

	static const TMap<EEquipmentType, const TArray<EEquipmentSlot>> TypeToSlotMap;
	
};
