// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HConditionAbility.h"

#include "Abilities/HAbilityStatics.h"
#include "StatusEffects/HConditionEffect.h"
#include "Characters/HCharacter.h"
#include "Utils/HEnumTools.h"

UHConditionAbility::UHConditionAbility()
{
	TargetType = CombineBits(EAbilityTarget::Actor, EAbilityTarget::Self);
}

void UHConditionAbility::FinishActorCast(AActor* TargetActor)
{
	ApplyCondition(TargetActor);
}

void UHConditionAbility::FinishSelfCast()
{
	ApplyCondition(AbilityComponent->GetCaster());
}

void UHConditionAbility::ApplyCondition(AActor* TargetActor)
{
	AHCharacter* Character = Cast<AHCharacter>(TargetActor);
	if (!Character)
	{
		return;
	}

	UHConditionEffect* Effect = UHAbilityStatics::CreateConditionEffect(Condition, this);
	Character->GetStatusEffectComponent()->ApplyEffect(Effect, AbilityComponent->GetCaster(), Duration);
}
