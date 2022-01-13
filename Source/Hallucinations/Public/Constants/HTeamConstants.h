#pragma once

UENUM()
enum class ETeam
{
	GoodGuys,
	BadGuys,
};

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EThreatStatus : uint8
{
	None = 0 UMETA(Hidden),
	Ally = 1 << 1,
	Enemy = 1 << 2,
	Neutral = 1 << 3,
};
ENUM_CLASS_FLAGS(EThreatStatus)