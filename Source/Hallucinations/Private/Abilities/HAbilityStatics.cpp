// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HAbilityStatics.h"
#include "Kismet/GameplayStatics.h"
#include "StatusEffects/HConditionEffect.h"

const FCollisionObjectQueryParams UHAbilityStatics::DefaultObjectQueryParams = {ECC_Pawn};


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

void UHAbilityStatics::GetActorsInRadius(TSet<AActor*>& OutActors, const UWorld* World, const FVector& Origin,
                                         float Radius, const FCollisionObjectQueryParams& ObjectQueryParams)
{
	TArray<FOverlapResult> Overlaps;
	World->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, ObjectQueryParams, FCollisionShape::MakeSphere(Radius));
	for (const FOverlapResult& Overlap : Overlaps)
	{
		OutActors.Add(Overlap.GetActor());
	}
}
