// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/HWeapon.h"
#include "HMeleeWeapon.generated.h"

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API AHMeleeWeapon : public AHWeapon
{
	GENERATED_BODY()

public:

	AHMeleeWeapon();
	
private:

	static const float RangeLeewayMultiplier;

	float RangeWithLeeway;

protected:

	virtual void BeginPlay() override;
	
public:

	virtual void AttackActor(AActor* TargetActor, bool bIsAbilityAttack, FAttackResult& OutResult) override;

	virtual void AttackLocation(const FVector& TargetLocation, bool bIsAbilityAttack, FAttackResult& OutResult) override;
};
