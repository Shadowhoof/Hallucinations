// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/HAbility.h"
#include "HAttackAbility.generated.h"

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API UHAttackAbility : public UHAbility
{
	GENERATED_BODY()

public:

	UHAttackAbility();

	void OnAttackPointReached(const FAttackResult& AttackResult);

protected:

	virtual void FinishActorAttack(AActor* TargetActor, const FVector& SpawnOrWeaponLocation);

	virtual void FinishLocationAttack(const FVector& TargetLocation, const FVector& SpawnOrWeaponLocation);
	
};
