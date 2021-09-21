// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/HSpellAbility.h"
#include "StatusEffects/HStatusEffectComponent.h"

#include "HConditionAbility.generated.h"

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API UHConditionAbility : public UHSpellAbility
{
	GENERATED_BODY()

public:

	UHConditionAbility();

	virtual void FinishActorCast(AActor* TargetActor) override;

	virtual void FinishSelfCast() override;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	EStatusCondition Condition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float Duration;

private:

	void ApplyCondition(AActor* TargetActor);
	
};
