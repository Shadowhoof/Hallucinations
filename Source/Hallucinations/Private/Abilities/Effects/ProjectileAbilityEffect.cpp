// Created by Dmitrii Korovin


#include "Abilities/Effects/ProjectileAbilityEffect.h"

#include "Projectiles/HAbstractProjectile.h"
#include "Core/HLogCategories.h"
#include "Kismet/GameplayStatics.h"

void UProjectileAbilityEffect::Apply(const FAbilityEffectParameters& Params)
{
	Super::Apply(Params);

	InstigatorActor = Params.InstigatorActor;
	InstigatorController = Params.InstigatorController;
	
	if (!ProjectileClass)
	{
		UE_LOG(LogAbilityEffect, Error, TEXT("No projectile specified for effect %s"), *GetName());
		return;
	}

	const FTransform SpawnTransform = Params.SpawnTransform;
	Projectile = Cast<AHAbstractProjectile>(UGameplayStatics::BeginDeferredActorSpawnFromClass(
		InstigatorActor, ProjectileClass, SpawnTransform, ESpawnActorCollisionHandlingMethod::AlwaysSpawn, InstigatorActor));
	if (!Projectile)
	{
		UE_LOG(LogAbilityEffect, Error, TEXT("Failed to spawn projectile for effect %s"), *GetName());
		return;
	}
	Projectile->Initialize(Speed, static_cast<EThreatStatus>(AffectedTargets));
	UGameplayStatics::FinishSpawningActor(Projectile, SpawnTransform);
	
	Projectile->SuccessfulCollisionEvent.AddUObject(this, &UProjectileAbilityEffect::OnSuccessfulCollision);
}

void UProjectileAbilityEffect::OnSuccessfulCollision(AActor* HitActor, const FVector& HitLocation,
	const FHitResult& HitResult)
{
	const FAbilityEffectParameters Params{InstigatorActor, InstigatorController, HitActor, HitLocation, HitResult};
	for (UAbilityEffect* Effect : Effects)
	{
		Effect->Apply(Params);
	}
}
