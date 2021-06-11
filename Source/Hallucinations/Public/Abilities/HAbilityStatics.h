// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "UObject/NoExportTypes.h"
#include "HAbilityStatics.generated.h"

/**
 *
 */
UCLASS()
class HALLUCINATIONS_API UHAbilityStatics : public UObject
{
	GENERATED_BODY()

public:

	static void SingleTargetDamage(AActor* TargetActor, float Damage, AActor* SourceActor, AController* Instigator,
								   TSubclassOf<UDamageType> DamageClass, const FHitResult& HitResult);

	static void AreaOfEffectDamage(const FVector& Origin, float Damage, float Radius, AActor* SourceActor,
								   AController* Instigator, TSubclassOf<UDamageType> DamageClass);

	static void AreaOfEffectDamageWithFalloff(const FVector& Origin, float Damage, float MinDamage, float InnerRadius, float OuterRadius,
											  AActor* SourceActor, AController* Instigator, TSubclassOf<UDamageType> DamageClass);
	
};
