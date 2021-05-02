// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HProjectileMovementComponent.h"
#include "Characters/HCharacter.h"
#include "Components/HHealthComponent.h"
#include "Actors/HProjectile.h"

UHProjectileMovementComponent::UHProjectileMovementComponent()
{
	ProjectileGravityScale = 0.f;
}

UProjectileMovementComponent::EHandleBlockingHitResult UHProjectileMovementComponent::HandleBlockingHit(
	const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining)
{
	AActor* HitActor = Hit.GetActor();
	if (UHHealthComponent::AreAllies(GetOwner()->GetInstigator(), HitActor))
	{
		AHAbstractProjectile* Projectile = Cast<AHAbstractProjectile>(GetOwner());
		Projectile->IgnoreActor(HitActor);
		return EHandleBlockingHitResult::AdvanceNextSubstep;
	}

	return Super::HandleBlockingHit(Hit, TimeTick, MoveDelta, SubTickTimeRemaining);
}

void UHProjectileMovementComponent::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta)
{
	if (UHHealthComponent::AreEnemies(GetOwner()->GetInstigator(), Hit.GetActor()))
	{
		Super::HandleImpact(Hit, TimeSlice, MoveDelta);
	}
}
