// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/HWeapon.h"
#include "HRangedWeapon.generated.h"

class AHProjectile;

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API AHRangedWeapon : public AHWeapon
{
	GENERATED_BODY()

public:

	AHRangedWeapon();
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	TSubclassOf<AHProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile")
	FName ProjectileSocketName;

	void ShootAtLocation(const FVector& Location, bool MatchSocketHeight) const;

public:
	
	virtual void AttackActor(AActor* const TargetActor) override;

	virtual void AttackLocation(const FVector& TargetLocation) override;
	
};
