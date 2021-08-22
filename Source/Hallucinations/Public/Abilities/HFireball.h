// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "HSpellAbility.h"
#include "HFireball.generated.h"

/**
 *
 */
UCLASS()
class HALLUCINATIONS_API UHFireball : public UHSpellAbility
{
	GENERATED_BODY()

	friend class AHFireballActor;

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

	virtual void FinishActorCast(AActor* TargetActor) override;

	virtual void FinishLocationCast(FVector TargetLocation) override;

private:

	IHAbilityActorInterface* CreateProjectile(FVector TargetLocation);

};
