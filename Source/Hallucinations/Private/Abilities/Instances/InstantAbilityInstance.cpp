// Created by Dmitrii Korovin


#include "Abilities/Instances/InstantAbilityInstance.h"

void UInstantAbilityInstance::Initialize(UHAbility* InAbility, const FAbilityEffectParameters& InEffectParams)
{
	Super::Initialize(InAbility, InEffectParams);

	ApplyEffects();
}
