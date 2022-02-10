// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Effects/AbilityEffect.h"
#include "UObject/Object.h"
#include "AbilityInstance.generated.h"

/**
 *  Unique instance of an ability whenever it is cast
 */
UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, Abstract)
class HALLUCINATIONS_API UAbilityInstance : public UObject
{
	GENERATED_BODY()

public:
	
	UAbilityInstance();

	virtual void Initialize(UHAbility* InAbility, const FAbilityEffectParameters& InEffectParams);

protected:

	UPROPERTY()
	UHAbility* Ability;
	
	FAbilityEffectParameters EffectParams;
	
protected:
	
	void ApplyEffects();
	
};
