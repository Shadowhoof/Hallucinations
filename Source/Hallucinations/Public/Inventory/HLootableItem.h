// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Core/HInteractable.h"
#include "GameFramework/Actor.h"
#include "HLootableItem.generated.h"


class UHInventoryItem;
struct FInventoryItem;


UCLASS()
class HALLUCINATIONS_API AHLootableItem : public AActor, public IHInteractable
{
	GENERATED_BODY()
	
public:	

	AHLootableItem();

	virtual void InteractWith(AHCharacter* Interactor) override;

	virtual float GetInteractionRange() const override;

	virtual FVector GetInteractableLocation() const override;

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static AHLootableItem* SpawnItemFromData(const UObject* WorldContextObject, TSubclassOf<AHLootableItem> ItemClass, const FInventoryItem& Data, const FTransform& Transform);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"))
	static AHLootableItem* SpawnItemFromObject(const UObject* WorldContextObject, TSubclassOf<AHLootableItem> ItemClass, UHInventoryItem* Item, const FTransform& Transform);
	
protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Data")
	UHInventoryItem* ItemData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

private:

	void Initialize(UHInventoryItem* ProvidedData);
	
};
