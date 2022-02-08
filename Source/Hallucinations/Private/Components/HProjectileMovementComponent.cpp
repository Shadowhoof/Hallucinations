// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HProjectileMovementComponent.h"
#include "Characters/HCharacter.h"
#include "Components/HHealthComponent.h"
#include "Projectiles/HProjectile.h"
#include "Utils/HUtils.h"

UHProjectileMovementComponent::UHProjectileMovementComponent()
{
	ProjectileGravityScale = 0.f;
}

void UHProjectileMovementComponent::SetAffectedTargets(EThreatStatus InAffectedTargets)
{
	AffectedTargets = InAffectedTargets;
}

UProjectileMovementComponent::EHandleBlockingHitResult UHProjectileMovementComponent::HandleBlockingHit(
	const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining)
{
	AActor* HitActor = Hit.GetActor();
	if (!AHAbstractProjectile::CanCollide(GetOwner()->GetInstigator(), HitActor, AffectedTargets))
	{
		AHAbstractProjectile* Projectile = Cast<AHAbstractProjectile>(GetOwner());
		Projectile->IgnoreActor(HitActor);
		return EHandleBlockingHitResult::AdvanceNextSubstep;
	}

	return Super::HandleBlockingHit(Hit, TimeTick, MoveDelta, SubTickTimeRemaining);
}

void UHProjectileMovementComponent::HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta)
{
	if (AHAbstractProjectile::CanCollide(GetOwner()->GetInstigator(), Hit.GetActor(), AffectedTargets))
	{
		Super::HandleImpact(Hit, TimeSlice, MoveDelta);
	}
}
