// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HFireballActor.h"
#include "DrawDebugHelpers.h"
#include "Abilities/HAbilityStatics.h"
#include "Abilities/HFireball.h"
#include "Components/HHealthComponent.h"
#include "Components/HProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Core/HLogCategories.h"

AHFireballActor::AHFireballActor()
{
}

void AHFireballActor::Initialize(UHAbility* AbilityDesc)
{
	UHFireball* FireballDesc = Cast<UHFireball>(AbilityDesc);
	const FVector Velocity = FVector(FireballDesc->Speed, 0.f, 0.f);
	MovementComponent->SetVelocityInLocalSpace(Velocity);
	MovementComponent->ProjectileGravityScale = 0.f;

	HitDamage = FireballDesc->HitDamage;
	ExplosionDamage = FireballDesc->ExplosionDamage;
	ExplosionRadius = FireballDesc->ExplosionRadius;
	bIsFalloff = FireballDesc->bIsFalloff;
}

void AHFireballActor::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                    const FHitResult& SweepResult)
{
	if (UHHealthComponent::AreAllies(GetInstigator(), OtherActor))
	{
		// fireball should go through allies
		return;
	}

	UE_LOG(LogAbility, Log, TEXT("Fireball impact on %s"), OtherActor ? *OtherActor->GetName() : TEXT("Null (BSP?)"));

	// apply damage
	UHAbilityStatics::SingleTargetDamage(OtherActor, HitDamage, GetInstigator(), GetInstigatorController(), 
										 UDamageType::StaticClass(), SweepResult);
	UHAbilityStatics::AreaOfEffectDamage(GetActorLocation(), ExplosionDamage, ExplosionRadius, GetInstigator(), 
										 GetInstigatorController(), UDamageType::StaticClass());

	// spawn explosion FX
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, GetActorLocation());
	
	Destroy();
}
