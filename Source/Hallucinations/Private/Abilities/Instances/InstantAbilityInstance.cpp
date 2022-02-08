// Created by Dmitrii Korovin


#include "Abilities/Instances/InstantAbilityInstance.h"

void UInstantAbilityInstance::Initialize(UHAbility* InAbility, const FAbilityTargetParameters& InTargetParams,
	AActor* SourceActor, const FTransform& SpawnTransform)
{
	Super::Initialize(InAbility, InTargetParams, SourceActor, SpawnTransform);

	ApplyEffectsWithBaseParams();
}
