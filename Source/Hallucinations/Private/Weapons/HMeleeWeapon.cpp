// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/HMeleeWeapon.h"

#include "Kismet/GameplayStatics.h"

const float AHMeleeWeapon::RangeLeewayMultiplier = 1.5f;

AHMeleeWeapon::AHMeleeWeapon()
{
	WeaponRange = 200.f;
}

void AHMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();

	RangeWithLeeway = WeaponRange * RangeLeewayMultiplier;
}

void AHMeleeWeapon::AttackActor(AActor* const TargetActor)
{
	if (TargetActor->GetDistanceTo(GetOwner()) <= RangeWithLeeway)
	{
		const FVector ToTargetDirection = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FHitResult HitResult;
		HitResult.Actor = TargetActor;
		HitResult.ImpactPoint = TargetActor->GetActorLocation();
		HitResult.ImpactNormal = -ToTargetDirection;
		UGameplayStatics::ApplyPointDamage(TargetActor, Damage, ToTargetDirection, HitResult, GetInstigatorController(), this, DamageType);
	}
}

void AHMeleeWeapon::AttackLocation(const FVector& TargetLocation)
{
	// melee ground attack does nothing
}
