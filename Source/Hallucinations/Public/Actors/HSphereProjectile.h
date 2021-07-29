// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/HAbstractProjectile.h"
#include "HSphereProjectile.generated.h"

class USphereComponent;

/**
 * Projectile with sphere collision and no mesh
 */
UCLASS()
class HALLUCINATIONS_API AHSphereProjectile : public AHAbstractProjectile
{
	GENERATED_BODY()

public:
	
	AHSphereProjectile();
	
protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	UParticleSystemComponent* TrailEffect;

	/** Effect size will be scaled between (1 - TrailRandomVariation) and (1 + TrailRandomVariation) */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	float TrailSizeVariation = 0.2;

	/** Radius parameter name for trail particle system */
	static const FName TrailSizeParameterName;

	UFUNCTION()
	virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
								bool bFromSweep, const FHitResult& SweepResult);

};
