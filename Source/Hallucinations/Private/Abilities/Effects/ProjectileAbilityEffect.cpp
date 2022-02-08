// Created by Dmitrii Korovin


#include "Abilities/Effects/ProjectileAbilityEffect.h"

#include "Projectiles/HAbstractProjectile.h"
#include "Core/HLogCategories.h"

void UProjectileAbilityEffect::Apply(AActor* InInstigatorActor, AController* InInstigatorController,
                                     const FAbilityTargetParameters& TargetParams, const FHitResult* HitResult)
{
	Super::Apply(InInstigatorActor, InInstigatorController, TargetParams, HitResult);

	InstigatorActor = InInstigatorActor;
	InstigatorController = InInstigatorController;
	
	if (!ProjectileClass)
	{
		UE_LOG(LogAbilityEffect, Error, TEXT("No projectile specified for effect %s"), *GetName());
		return;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = InInstigatorActor;
	if (APawn* Pawn = Cast<APawn>(InInstigatorActor))
	{
		SpawnParams.Instigator = Pawn;
	}
	
	Projectile = Cast<AHAbstractProjectile>(GetWorld()->SpawnActor(ProjectileClass, &InInstigatorActor->GetTransform(), SpawnParams));
	if (!Projectile)
	{
		UE_LOG(LogAbilityEffect, Error, TEXT("Failed to spawn projectile for effect %s"), *GetName());
		return;
	}
	Projectile->Initialize(Speed, static_cast<EThreatStatus>(AffectedTargets));
	Projectile->SuccessfulCollisionEvent.AddUObject(this, &UProjectileAbilityEffect::OnSuccessfulCollision);
}

void UProjectileAbilityEffect::OnSuccessfulCollision(AActor* HitActor, const FVector& HitLocation,
	const FHitResult& HitResult)
{
	const FAbilityTargetParameters TargetParams{HitActor, HitLocation};
	for (UAbilityEffect* Effect : Effects)
	{
		Effect->Apply(InstigatorActor, InstigatorController, TargetParams, &HitResult);
	}
}
