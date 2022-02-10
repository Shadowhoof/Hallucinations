// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "AbilityEffect.h"
#include "StatusEffects/HStatusEffectComponent.h"
#include "ConditionAbilityEffect.generated.h"


/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API UConditionAbilityEffect : public UAbilityEffect
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Ability")
	EStatusCondition Condition;

	UPROPERTY(EditAnywhere, Category = "Ability")
	float Duration;

protected:

	virtual void ApplyToActor(AActor* InInstigatorActor, AController* InInstigatorController, AActor* HitActor, const FHitResult& HitResult) override;
	
};
