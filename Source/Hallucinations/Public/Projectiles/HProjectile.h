// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAbstractProjectile.h"
#include "HProjectile.generated.h"


// Holds projectile data filled by the shooter
struct FProjectileData
{
	// Base damage that will be dealt by the projectiles
	float Damage;

	// Damage type of projectile
	TSubclassOf<UDamageType> DamageType;
};


UCLASS()
class HALLUCINATIONS_API AHProjectile : public AHAbstractProjectile
{
	GENERATED_BODY()
	
public:
	
	AHProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);

	virtual void OnSuccessfulCollision(AActor* HitActor, const FHitResult& HitResult) override;
	
public:	
	
	FProjectileData Data;

	virtual void IgnoreActor(AActor* Actor) override;

};
