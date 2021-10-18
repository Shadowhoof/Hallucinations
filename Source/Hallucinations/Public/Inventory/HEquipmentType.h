#pragma once

#include "HEquipmentType.generated.h"

/**
 * Equipment slot of an equipment component.
 */
UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	None,
	Head,
	Chest,
	Waist,
	Hands,
	Feet,
	Ring1,
	Ring2,
	Amulet,
	MainHandWeapon,
	OffHandWeapon,
};
ENUM_RANGE_BY_FIRST_AND_LAST(EEquipmentSlot, EEquipmentSlot::Head, EEquipmentSlot::OffHandWeapon);


/** *
 * Equipment type of an item. 
 */
UENUM(BlueprintType)
enum class EEquipmentType : uint8
{
	None,
	Head,
	Chest,
	Waist,
	Hands,
	Feet,
	Ring,
	Amulet,
	MainHand,
	OffHand,
	AnyHand,
};