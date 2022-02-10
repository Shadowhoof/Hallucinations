// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HAbstractProjectile.generated.h"

class UHProjectileMovementComponent;
enum class EThreatStatus : uint8;

UCLASS()
class HALLUCINATIONS_API AHAbstractProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	
	AHAbstractProjectile();

	virtual void Tick(float DeltaTime) override;

	virtual void IgnoreActor(AActor* Actor);

	/**
	 * @brief Initializes projectile with custom values.
	 * @param Speed initial projectile speed, if less than or equal to zero then this value is ignored
	 * @param InAffectedTargets targets that projectile will be able to collide with in relation to the owner of the projectile
	 */
	virtual void Initialize(float Speed, EThreatStatus InAffectedTargets);
	
	DECLARE_EVENT_ThreeParams(AHAbstractProjectile, FProjectileCollisionEvent, AActor*, const FVector&, const FHitResult&)
	FProjectileCollisionEvent SuccessfulCollisionEvent;

	static bool CanCollide(const AActor* ProjectileInstigator, const AActor* HitActor, EThreatStatus AffectedProjectileTargets);
	
protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	UHProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	UParticleSystemComponent* TrailFXComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	UParticleSystem* ImpactFX;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float MaxRange = 10000.f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float DistanceTraveled = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Bitmask, BitmaskEnum = "EThreatStatus"))
	uint8 AffectedTargets;
	
	virtual void OnSuccessfulCollision(AActor* HitActor, const FHitResult& HitResult);

	virtual void SpawnImpactFX(const FVector& Location);

};
