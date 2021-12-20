// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define ECC_Click ECC_GameTraceChannel1
#define ECC_Projectile ECC_GameTraceChannel2
#define ECC_FadeTrigger ECC_GameTraceChannel3

namespace HallucinationsConstants
{
	// invalid values
	
	const FVector InvalidVector(FLT_MAX, FLT_MAX, FLT_MAX);

	const FString PlayerStartTag("PlayerStartTag");
	const FString DefaultPlayerStartTag("Default");
}

struct FHConstants
{
	
	static const FName ArrowSocketName;
	static const FName SpellSocketName;
	
	static const FVector NullVector;

	static const float CapsuleHalfHeight;
};
