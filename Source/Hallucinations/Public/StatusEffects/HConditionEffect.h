// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/HStatusEffect.h"
#include "HConditionEffect.generated.h"

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API UHConditionEffect : public UHStatusEffect
{
	GENERATED_BODY()

public:

	UHConditionEffect();
	
	void Initialize(EStatusCondition StatusCondition);
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "Status")
	EStatusCondition Condition;

	virtual void Begin(AActor* Affected, AActor* Source, float Duration) override;

	virtual void End() override;

	virtual uint8 GetActiveConditions() const override;
	
};
