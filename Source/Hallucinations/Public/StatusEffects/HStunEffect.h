// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/HStatusEffect.h"
#include "HStunEffect.generated.h"

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API UHStunEffect : public UHStatusEffect
{
	GENERATED_BODY()

public:
	
	virtual void Begin(AActor* Affected, AActor* Source, float Duration) override;

	virtual void End() override;

	virtual uint8 GetActiveConditions() const override;
};
