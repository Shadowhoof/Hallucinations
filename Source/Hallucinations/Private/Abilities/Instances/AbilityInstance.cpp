// Created by Dmitrii Korovin


#include "Abilities/Instances/AbilityInstance.h"

#include "Abilities/HAbility.h"
#include "Abilities/Effects/AbilityEffect.h"
#include "Core/HLogCategories.h"

UAbilityInstance::UAbilityInstance()
{
}

void UAbilityInstance::Initialize(UHAbility* InAbility, const FAbilityEffectParameters& InEffectParams)
{
	Ability = InAbility;
	EffectParams = InEffectParams;
}

void UAbilityInstance::ApplyEffects()
{
	ensureMsgf(Ability, TEXT("No ability assigned for ability actor %s"), *GetName());
	for (UAbilityEffect* Effect : Ability->GetEffects())
	{
		if (!Effect)
		{
			UE_LOG(LogAbilityEffect, Warning, TEXT("One of the effects in ability %s is nullptr"), *Ability->GetSkillNameAsString())
			continue;
		}
		Effect->Apply(EffectParams);
	}
}
