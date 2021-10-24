// Fill out your copyright notice in the Description page of Project Settings.


#include "Leveling/HAttributeComponent.h"

#include "Inventory/HEquipmentComponent.h"
#include "Inventory/HInventoryItem.h"
#include "Core/HLogCategories.h"
#include "Utils/HEnumTools.h"

DEFINE_LOG_CATEGORY(LogAttributes);


const float UHAttributeComponent::InvalidAttributeValue = FLT_MIN;


UAttributeSet::UAttributeSet()
{
}

float UAttributeSet::GetAttributeValue(EAttributeType Type)
{
	FAttributeData* AttributeDataPtr = Attributes.Find(Type);
	if (!AttributeDataPtr)
	{
		return UHAttributeComponent::InvalidAttributeValue;
	}

	return AttributeDataPtr->CurrentValue;
}

void UAttributeSet::Initialize(UHEquipmentComponent* EquipmentComponent)
{
	Recalculate(EquipmentComponent);
}

void UAttributeSet::Recalculate(UHEquipmentComponent* EquipmentComponent)
{
	TMap<EAttributeType, float> ItemBonuses;
	for (const UHInventoryItem* Item : EquipmentComponent->GetEquippedItems())
	{
		for (const TTuple<EAttributeType, float>& StatTuple : Item->GetData().Stats)
		{
			ItemBonuses.FindOrAdd(StatTuple.Key) += StatTuple.Value;
		}
	}

	for (TTuple<EAttributeType, FAttributeData>& Item : Attributes)
	{
		FAttributeData& AttributeData = Item.Value;
		float* EquipmentValuePtr = ItemBonuses.Find(Item.Key);
		const float EquipmentValue = EquipmentValuePtr ? *EquipmentValuePtr : 0.f;
		AttributeData.CurrentValue = AttributeData.BaseValue + EquipmentValue;
	}
}

UHAttributeComponent::UHAttributeComponent()
{
}

float UHAttributeComponent::GetAttributeValue(EAttributeType Type)
{
	return AttributeSet->GetAttributeValue(Type);
}

void UHAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	UHEquipmentComponent* Equipment = Cast<UHEquipmentComponent>(GetOwner()->GetComponentByClass(UHEquipmentComponent::StaticClass()));
	if (Equipment)
	{
		EquipmentComponent = Equipment;
		EquipmentComponent->OnItemEquipped.AddDynamic(this, &UHAttributeComponent::OnItemEquipped);
		EquipmentComponent->OnItemUnequipped.AddDynamic(this, &UHAttributeComponent::OnItemUnequipped);
	}
	else
	{
		UE_LOG(LogAttributes, Error, TEXT("No equipment component found while initializing attribute component, owner: %s"), *GetOwner()->GetName());
	}

	AttributeSet = NewObject<UAttributeSet>(this, AttributeSetClass);
	AttributeSet->Initialize(EquipmentComponent.Get());
}

void UHAttributeComponent::OnItemEquipped(UHInventoryItem* Item)
{
	ensure(EquipmentComponent.IsValid());
	UE_LOG(LogAttributes, Log, TEXT("Item equipped: %s"), *Item->GetData().Name.ToString());
	const TMap<EAttributeType, float>& Attributes = Item->GetData().Stats;
	for (const TTuple<EAttributeType, float>& AttributeTuple : Attributes)
	{
		UE_LOG(LogAttributes, Log, TEXT("Attribute %s, value: %.2f"), *EnumToString(AttributeTuple.Key), AttributeTuple.Value)
	}
	AttributeSet->Recalculate(EquipmentComponent.Get());
}

void UHAttributeComponent::OnItemUnequipped(UHInventoryItem* Item)
{
	ensure(EquipmentComponent.IsValid());
	UE_LOG(LogAttributes, Log, TEXT("Item unequipped: %s"), *Item->GetData().Name.ToString());
	const TMap<EAttributeType, float>& Attributes = Item->GetData().Stats;
	for (const TTuple<EAttributeType, float>& AttributeTuple : Attributes)
	{
		UE_LOG(LogAttributes, Log, TEXT("Attribute %s, value: %.2f"), *EnumToString(AttributeTuple.Key), AttributeTuple.Value)
	}
	AttributeSet->Recalculate(EquipmentComponent.Get());
}

