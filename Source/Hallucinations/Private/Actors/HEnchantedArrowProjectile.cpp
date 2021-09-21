// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/HEnchantedArrowProjectile.h"

#include "Abilities/HAbilityStatics.h"
#include "Abilities/HEnchantedArrow.h"
#include "Particles/ParticleSystemComponent.h"
#include "StatusEffects/HConditionEffect.h"
#include "Utils/HEnumTools.h"
#include "Utils/HLogUtils.h"

AHEnchantedArrowProjectile::AHEnchantedArrowProjectile()
{
	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailEffect"));
	TrailEffect->SetupAttachment(RootComponent);
}

void AHEnchantedArrowProjectile::Initialize(UHAbility* AbilityDesc, AActor* SourceActor)
{
	UHEnchantedArrow* Ability = Cast<UHEnchantedArrow>(AbilityDesc);
	Damage = Ability->Damage;
	Condition = Ability->Condition;
	ConditionDuration = Ability->ConditionDuration;
}

void AHEnchantedArrowProjectile::OnSuccessfulHit(AActor* HitActor, const FHitResult& HitResult)
{
	LOG_TEMP("Damage: %.2f, Condition: %d", Damage, BIT_AS_INT(Condition));
	UHAbilityStatics::DealSingleTargetDamage(HitActor, Damage, this, GetInstigatorController(), Data.DamageType, HitResult);
	UHStatusEffectComponent* Component = Cast<UHStatusEffectComponent>(HitActor->GetComponentByClass(UHStatusEffectComponent::StaticClass()));
	if (Component)
	{
		UHConditionEffect* ConditionEffect = UHAbilityStatics::CreateConditionEffect(Condition, Component);
		Component->ApplyEffect(ConditionEffect, GetInstigator(), ConditionDuration);
	}
}
