// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Actors/HSphereProjectile.h"
#include "Abilities/HAbilityActorInterface.h"
#include "HFireballActor.generated.h"

class USphereComponent;
class UHFireball;
class UHProjectileMovementComponent;
class AHSphereProjectile;
class IHAbilityActorInterface;

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API AHFireballActor : public AHSphereProjectile, public IHAbilityActorInterface
{
	GENERATED_BODY()

public:

	AHFireballActor();

protected:
	
	float HitDamage;
	float ExplosionDamage;
	float ExplosionRadius;
	bool bIsFalloff;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	UParticleSystem* ExplosionEffect;

public:

	virtual void Initialize(UHAbility* AbilityDesc, AActor* SourceActor) override;

	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

};
