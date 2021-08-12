#pragma once

#include "HAL/Platform.h"

inline WIDECHAR* BoolToString(bool Value)
{
	return Value ? TEXT("true") : TEXT("false");
}

#define LOG_TEMP(RawText, ...) UE_LOG(LogTemp, Log, TEXT(RawText), ##__VA_ARGS__)