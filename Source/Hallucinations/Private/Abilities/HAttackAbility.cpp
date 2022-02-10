// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HAttackAbility.h"

#include "Characters/HCharacter.h"
#include "Components/HAttackComponent.h"
#include "Utils/HEnumTools.h"
#include "Utils/HUtils.h"

UHAttackAbility::UHAttackAbility()
{
	Type = EAbilityType::Attack;
}

void UHAttackAbility::OnAttackPointReached(const FAttackResult& AttackResult)
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
	const FTransform SpawnTransform = GetSpawnTransform(SpawnOrWeaponLocation, TargetActor->GetActorLocation());
	CreateInstance(SpawnTransform, TargetActor);
}

void UHAttackAbility::FinishLocationAttack(const FVector& TargetLocation, const FVector& SpawnOrWeaponLocation)
{
	const FTransform SpawnTransform = GetSpawnTransform(SpawnOrWeaponLocation, TargetLocation);
	CreateInstance(SpawnTransform, TargetLocation);
}

FTransform UHAttackAbility::GetSpawnTransform(const FVector& SpawnLocation, FVector TargetLocation)
{
	TargetLocation.Z = SpawnLocation.Z;
	const FRotator Rotation = (TargetLocation - SpawnLocation).Rotation();
	return FTransform(Rotation, SpawnLocation);
}
