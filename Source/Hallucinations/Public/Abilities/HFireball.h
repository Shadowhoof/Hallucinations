// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/HAbility.h"
#include "HFireball.generated.h"

/**
 *
 */
UCLASS()
class HALLUCINATIONS_API UHFireball : public UHAbility
{
	GENERATED_BODY()

	friend class AHFireballImpl;

public:

	UHFireball();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float HitDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float ExplosionDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float ExplosionRadius;

	/** Indicates whether damage falls off with distance */
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	bool bIsFalloff;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float Speed;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float Radius;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AHFireballImpl> FireballClass;

	virtual void FinishActorCast(UHAbilityComponent* Context, AActor* TargetActor) override;

	virtual void FinishLocationCast(UHAbilityComponent* Context, FVector TargetLocation) override;

private:

	void CreateProjectile(UHAbilityComponent* Context, FVector TargetLocation);

};
