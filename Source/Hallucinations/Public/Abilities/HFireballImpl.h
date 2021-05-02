// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/HAbstractProjectile.h"
#include "HFireballImpl.generated.h"

class USphereComponent;
class UHFireball;
class UHProjectileMovementComponent;

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API AHFireballImpl : public AHAbstractProjectile
{
	GENERATED_BODY()

public:

	AHFireballImpl();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Collision")
	USphereComponent* CollisionComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystemComponent* TrailEffect;
	
	float HitDamage;
	float ExplosionDamage;
	float ExplosionRadius;
	bool bIsFalloff;
	
public:

	void Initialize(UHFireball& AbilityDesc);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
