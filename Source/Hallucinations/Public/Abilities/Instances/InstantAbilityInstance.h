// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "AbilityInstance.h"
#include "InstantAbilityInstance.generated.h"

/**
 *  Ability instance that applies ability effects on next tick after its creation.
 */
UCLASS()
class HALLUCINATIONS_API UInstantAbilityInstance : public UAbilityInstance
{
	GENERATED_BODY()

public:
	
	virtual void Initialize(UHAbility* InAbility, const FAbilityEffectParameters& InEffectParams) override;
};
