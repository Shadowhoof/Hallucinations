// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "HRangedAttackAbility.h"
#include "StatusEffects/HStatusEffectComponent.h"

#include "HEnchantedArrow.generated.h"

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API UHEnchantedArrow : public UHRangedAttackAbility
{
	GENERATED_BODY()

	friend class AHEnchantedArrowProjectile;

public:

	UHEnchantedArrow();
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "StatusEffect")
	EStatusCondition Condition;

	UPROPERTY(EditDefaultsOnly, Category = "StatusEffect")
	float ConditionDuration;
	
};
