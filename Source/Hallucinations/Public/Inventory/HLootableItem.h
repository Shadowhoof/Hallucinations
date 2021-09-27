// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Core/HInteractable.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "HLootableItem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInventory, Log, All)

UENUM(BlueprintType)
enum class EItemQuality : uint8
{
	Common,
	Rare,
	Epic,
	Unique
};


USTRUCT(BlueprintType)
struct FInventoryItem : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EItemQuality Quality;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* Mesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon;

};


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
	static AHLootableItem* SpawnItem(const UObject* WorldContextObject, TSubclassOf<AHLootableItem> ItemClass, const FInventoryItem& Data, const FTransform& Transform);

protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	FInventoryItem ItemData;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

private:

	void Initialize(const FInventoryItem ProvidedData);
	
};
