// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HRangedAttackAbility.h"
#include "Characters/HCharacter.h"
#include "Actors/HProjectile.h"
#include "Core/HLogCategories.h"

void UHRangedAttackAbility::FinishActorAttack(AActor* TargetActor, const FVector& SpawnOrWeaponLocation)
{
	CreateProjectile(TargetActor->GetTargetLocation(), SpawnOrWeaponLocation, false);
}

void UHRangedAttackAbility::FinishLocationAttack(const FVector& TargetLocation, const FVector& SpawnOrWeaponLocation)
{
	CreateProjectile(TargetLocation, SpawnOrWeaponLocation, true);
}

AHProjectile* UHRangedAttackAbility::CreateProjectile(const FVector& TargetLocation, const FVector& SpawnLocation, bool MatchSocketHeight)
{
	FRotator Rotation = (TargetLocation - SpawnLocation).Rotation();
	if (MatchSocketHeight)
	{
		Rotation.Pitch = 0.f;
	}
	
	AHCharacter* Caster = AbilityComponent->GetCaster();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Caster;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = Caster;
	AHProjectile* Projectile = Cast<AHProjectile>(GetWorld()->SpawnActor(ProjectileClass, &SpawnLocation, &Rotation, SpawnParams));
	if (!Projectile)
	{
		UE_LOG(LogAbility, Log, TEXT("Failed to create projectile of class %s for ability %s"), *ProjectileClass->GetName(), *GetSkillNameAsString())
		return nullptr;
	}

	Projectile->Data = GetProjectileData();
	return Projectile;
}

FProjectileData UHRangedAttackAbility::GetProjectileData() const
{
	FProjectileData Data;
	Data.Damage = AbilityComponent->GetCaster()->GetCurrentDamage();
	Data.DamageType = UDamageType::StaticClass();
	return Data;
}
