// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HHeadBlowActor.h"
#include "Abilities/HHeadBlow.h"
#include "StatusEffects/HStatusEffectComponent.h"
#include "Core/HLogCategories.h"
#include "StatusEffects/HStunEffect.h"

void AHHeadBlowActor::Initialize(UHAbility* AbilityDesc, AActor* SourceActor, AActor* TargetActor)
{
	UHHeadBlow* Ability = Cast<UHHeadBlow>(AbilityDesc);
	UHStatusEffectComponent* Component = Cast<UHStatusEffectComponent>(TargetActor->GetComponentByClass(UHStatusEffectComponent::StaticClass()));
	if (!Component)
	{
		UE_LOG(LogAbility, Log, TEXT("No StatusEffectComponent was found for target actor %s"), *TargetActor->GetName());
		return;
	}

	Component->ApplyEffect(UHStunEffect::StaticClass(), SourceActor, Ability->Duration);
	Destroy();
}

