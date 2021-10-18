#pragma once

#include "Engine/DataTable.h"
#include "Inventory/HInventoryComponent.h"
#include "HEquipmentType.h"
#include "HInventoryItem.generated.h"

class UTexture2D;
class UStaticMesh;


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

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEquipmentType EquipmentType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FInventoryDimensions Dimensions;
	
};


UCLASS(Blueprintable)
class HALLUCINATIONS_API UHInventoryItem : public UObject
{
	GENERATED_BODY()

public:

	UHInventoryItem();

	const FInventoryItem& GetData() const;

	static UHInventoryItem* CreateItem(const FInventoryItem& Data);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FInventoryItem Data;

};