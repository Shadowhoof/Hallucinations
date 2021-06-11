// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/HAbility.h"
#include "HFlamestrike.generated.h"

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API UHFlamestrike : public UHAbility
{
	GENERATED_BODY()

	friend class AHFlamestrikeActor;
	
public:

	UHFlamestrike();
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float Radius;

	virtual void FinishLocationCast(UHAbilityComponent* Context, FVector TargetLocation) override;
	
};
