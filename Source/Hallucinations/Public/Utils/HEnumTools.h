#pragma once

#define TEST_BIT(Mask, Bit) (Mask & static_cast<uint8>(Bit))
#define SET_BIT(Mask, Bit) (Mask |= static_cast<uint8>(Bit))
#define UNSET_BIT(Mask, Bit) (Mask &= ~static_cast<uint8>(Bit))
#define BIT_AS_INT(Bit) (static_cast<uint8>(Bit))

template<typename... Args>
uint8 CombineBits(Args... Bits)
{
	return (static_cast<uint8>(Bits) + ...);
}

template<typename T>
FString EnumAsString(T Value)
{
	return StaticEnum<T>()->GetValueAsString(Value);
}
