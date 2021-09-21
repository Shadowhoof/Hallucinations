// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HAbilityStatics.h"
#include "Kismet/GameplayStatics.h"
#include "StatusEffects/HConditionEffect.h"

void UHAbilityStatics::DealSingleTargetDamage(AActor* TargetActor, float Damage, AActor* SourceActor,
                                              AController* Instigator, TSubclassOf<UDamageType> DamageClass, const FHitResult& HitResult)
{
	const FVector HitDirection = (HitResult.TraceEnd - HitResult.TraceStart).GetSafeNormal();
	UGameplayStatics::ApplyPointDamage(TargetActor, Damage, HitDirection, HitResult, Instigator, SourceActor, DamageClass);
}

void UHAbilityStatics::DealAreaOfEffectDamage(const FVector& Origin, float Damage, float Radius,
	AActor* SourceActor, AController* Instigator, TSubclassOf<UDamageType> DamageClass)
{
	// TODO - write own damage system so custom filters can be applied on actors
	const TArray<AActor*> IgnoredActors;
	UGameplayStatics::ApplyRadialDamage(SourceActor, Damage, Origin, Radius, DamageClass, IgnoredActors, 
										SourceActor, Instigator, true);
}

void UHAbilityStatics::DealAreaOfEffectDamageWithFalloff(const FVector& Origin, float Damage, float MinDamage, float InnerRadius,
													 float OuterRadius, AActor* SourceActor, AController* Instigator, 
													 TSubclassOf<UDamageType> DamageClass)
{
	const TArray<AActor*> IgnoredActors;
	UGameplayStatics::ApplyRadialDamageWithFalloff(SourceActor, Damage, MinDamage, Origin, InnerRadius, OuterRadius,
												   1.0, DamageClass, IgnoredActors, SourceActor, Instigator);
}

UHConditionEffect* UHAbilityStatics::CreateConditionEffect(EStatusCondition Condition, UObject* Outer)
{
	UHConditionEffect* ConditionEffect = NewObject<UHConditionEffect>(Outer, UHConditionEffect::StaticClass());
	ConditionEffect->Initialize(Condition);
	return ConditionEffect;
}
