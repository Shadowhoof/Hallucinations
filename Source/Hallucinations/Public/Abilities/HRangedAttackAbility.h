// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/HAttackAbility.h"
#include "Actors/HProjectile.h"

#include "HRangedAttackAbility.generated.h"

class AHProjectile;
/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API UHRangedAttackAbility : public UHAttackAbility
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TSubclassOf<AHProjectile> ProjectileClass;

	virtual void FinishActorAttack(AActor* TargetActor, const FVector& SpawnOrWeaponLocation) override;

	virtual void FinishLocationAttack(const FVector& TargetLocation, const FVector& SpawnOrWeaponLocation) override;

	AHProjectile* CreateProjectile(const FVector& TargetLocation, const FVector& SpawnLocation, bool MatchSocketHeight);

	virtual FProjectileData GetProjectileData() const;
};
