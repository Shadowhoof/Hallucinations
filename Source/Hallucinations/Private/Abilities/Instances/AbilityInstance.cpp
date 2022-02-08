// Created by Dmitrii Korovin


#include "Abilities/Instances/AbilityInstance.h"

#include "Abilities/HAbility.h"
#include "Abilities/Effects/AbilityEffect.h"

UAbilityInstance::UAbilityInstance()
{
}

void UAbilityInstance::Initialize(UHAbility* InAbility, const FAbilityTargetParameters& InTargetParams,
	AActor* SourceActor, const FTransform& SpawnTransform)
{
	Ability = InAbility;
	InstigatorActor = SourceActor;
	InstigatorController = InstigatorActor ? InstigatorActor->GetInstigatorController() : nullptr;
	TargetParams = InTargetParams;
}

void UAbilityInstance::ApplyEffects(const FAbilityTargetParameters& HitParams)
{
	ensureMsgf(Ability, TEXT("No ability assigned for ability actor %s"), *GetName());
	for (UAbilityEffect* Effect : Ability->GetEffects())
	{
		Effect->Apply(InstigatorActor, InstigatorController, HitParams, nullptr);
	}
}

void UAbilityInstance::ApplyEffectsWithBaseParams()
{
	ApplyEffects(TargetParams);
}
