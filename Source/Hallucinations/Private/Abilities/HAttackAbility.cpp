// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HAttackAbility.h"

#include "Characters/HCharacter.h"
#include "Components/HAttackComponent.h"
#include "Core/HLogCategories.h"
#include "Utils/HEnumTools.h"
#include "Utils/HUtils.h"

UHAttackAbility::UHAttackAbility()
{
	Type = EAbilityType::Attack;
}

void UHAttackAbility::OnAttackFinished(const FAttackResult& AttackResult)
{
	if (TEST_BIT(TargetType, EAbilityTarget::Actor) && AttackResult.Actor.IsValid())
	{
		FinishActorAttack(AttackResult.Actor.Get(), AttackResult.SpawnOrWeaponLocation);
	}
	else if (TEST_BIT(TargetType, EAbilityTarget::Point) && UHUtils::IsVectorValid(AttackResult.Location))
	{
		FinishLocationAttack(AttackResult.Location, AttackResult.SpawnOrWeaponLocation);
	}

	StartCooldown();
}

void UHAttackAbility::FinishActorAttack(AActor* TargetActor, const FVector& SpawnOrWeaponLocation)
{
}

void UHAttackAbility::FinishLocationAttack(const FVector& TargetLocation, const FVector& SpawnOrWeaponLocation)
{
}
