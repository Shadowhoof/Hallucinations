// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HAbilityStatics.h"
#include "Kismet/GameplayStatics.h"

void UHAbilityStatics::SingleTargetDamage(AActor* TargetActor, float Damage, AActor* SourceActor,
	AController* Instigator, TSubclassOf<UDamageType> DamageClass, const FHitResult& HitResult)
{
	const FVector HitDirection = (HitResult.TraceEnd - HitResult.TraceStart).GetSafeNormal();
	UGameplayStatics::ApplyPointDamage(TargetActor, Damage, HitDirection, HitResult, Instigator, SourceActor, DamageClass);
}

void UHAbilityStatics::AreaOfEffectDamage(const FVector& Origin, float Damage, float Radius,
	AActor* SourceActor, AController* Instigator, TSubclassOf<UDamageType> DamageClass)
{
	const TArray<AActor*> IgnoredActors;
	UGameplayStatics::ApplyRadialDamage(SourceActor, Damage, Origin, Radius, DamageClass, IgnoredActors, 
										SourceActor, Instigator, true);
}

void UHAbilityStatics::AreaOfEffectDamageWithFalloff(const FVector& Origin, float Damage, float MinDamage, float InnerRadius,
													 float OuterRadius, AActor* SourceActor, AController* Instigator, 
													 TSubclassOf<UDamageType> DamageClass)
{
	const TArray<AActor*> IgnoredActors;
	UGameplayStatics::ApplyRadialDamageWithFalloff(SourceActor, Damage, MinDamage, Origin, InnerRadius, OuterRadius,
												   1.0, DamageClass, IgnoredActors, SourceActor, Instigator);
}
