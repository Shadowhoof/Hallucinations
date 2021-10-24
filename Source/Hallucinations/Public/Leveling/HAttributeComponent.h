// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "HAttributeType.h"
#include "HAttributeComponent.generated.h"


class UHEquipmentComponent;
class UHInventoryItem;


USTRUCT(BlueprintType)
struct FAttributeData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attribute", meta = (ClampMin = 1))
	float BaseValue;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attribute", meta = (ClampMin = 1))
	float CurrentValue;

};


UCLASS(Blueprintable)
class HALLUCINATIONS_API UAttributeSet : public UObject
{

	GENERATED_BODY()
	
public:

	UAttributeSet();

	float GetAttributeValue(EAttributeType Type);

	void Initialize(UHEquipmentComponent* EquipmentComponent);

	void Recalculate(UHEquipmentComponent* EquipmentComponent);
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attribute")
	TMap<EAttributeType, FAttributeData> Attributes;
	
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HALLUCINATIONS_API UHAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UHAttributeComponent();

	UFUNCTION(BlueprintCallable)
	float GetAttributeValue(EAttributeType Type);

	// the value returned if attribute doesn't exist in the set
	static const float InvalidAttributeValue;
	
	
protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute Set")
	TSubclassOf<UAttributeSet> AttributeSetClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Attribute Set")
	UAttributeSet* AttributeSet;
	
	TWeakObjectPtr<UHEquipmentComponent> EquipmentComponent;

	UFUNCTION()
	void OnItemEquipped(UHInventoryItem* Item);

	UFUNCTION()
	void OnItemUnequipped(UHInventoryItem* Item);
	
};
