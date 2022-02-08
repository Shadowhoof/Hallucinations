// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "StatusEffects/HStatusEffectComponent.h"
#include "UObject/NoExportTypes.h"
#include "HAbilityStatics.generated.h"

class UHConditionEffect;

/**
 *  Helper static functions for various common gameplay cases.
 */
UCLASS()
class HALLUCINATIONS_API UHAbilityStatics : public UObject
{
	GENERATED_BODY()

public:

	static void DealSingleTargetDamage(AActor* TargetActor, float Damage, AActor* SourceActor, AController* Instigator,
								   TSubclassOf<UDamageType> DamageClass, const FHitResult& HitResult);

	static void DealAreaOfEffectDamage(const FVector& Origin, float Damage, float Radius, AActor* SourceActor,
								   AController* Instigator, TSubclassOf<UDamageType> DamageClass);

	static void DealAreaOfEffectDamageWithFalloff(const FVector& Origin, float Damage, float MinDamage, float InnerRadius, float OuterRadius,
											  AActor* SourceActor, AController* Instigator, TSubclassOf<UDamageType> DamageClass);

	static UHConditionEffect* CreateConditionEffect(EStatusCondition Condition, UObject* Outer);

	static void GetActorsInRadius(TSet<AActor*>& OutActors, const UWorld* World, const FVector& Origin,
	                              float Radius, const FCollisionObjectQueryParams& ObjectQueryParams = DefaultObjectQueryParams);

public:

	static const FCollisionObjectQueryParams DefaultObjectQueryParams;
	
};
