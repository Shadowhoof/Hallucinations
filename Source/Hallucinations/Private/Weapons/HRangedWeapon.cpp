// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/HRangedWeapon.h"
#include "HConstants.h"
#include "Actors/HProjectile.h"
#include "Characters/HCharacter.h"
#include "Components/HAttackComponent.h"

AHRangedWeapon::AHRangedWeapon()
{
	ProjectileClass = AHProjectile::StaticClass();
	ProjectileSocketName = FHConstants::ArrowSocketName;
}

void AHRangedWeapon::ShootAtLocation(const FVector& Location, bool MatchSocketHeight, const FVector& SpawnLocation)
{
	AHCharacter* Character = Cast<AHCharacter>(GetOwner());
	FRotator Rotation = (Location - SpawnLocation).Rotation();
	if (MatchSocketHeight)
	{
		Rotation.Pitch = 0.f;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Character;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = this;
	AHProjectile* Projectile = Cast<AHProjectile>(GetWorld()->SpawnActor(ProjectileClass, &SpawnLocation, &Rotation, SpawnParams));
	if (Projectile)
	{
		FProjectileData ProjectileData;
		ProjectileData.Damage = Character->GetCurrentDamage();
		ProjectileData.DamageType = DamageType;
		Projectile->Data = ProjectileData;
	}
}

FVector AHRangedWeapon::GetProjectileSpawnLocation()
{
	AHCharacter* Character = Cast<AHCharacter>(GetOwner());
	return ProjectileSocketName.IsNone() ? GetActorLocation() : Character->GetMesh()->GetSocketLocation(ProjectileSocketName);
}

void AHRangedWeapon::AttackActor(AActor* TargetActor, bool bIsAbilityAttack, FAttackResult& OutResult)
{
	FVector TargetLocation = TargetActor->GetTargetLocation(GetOwner());
	FVector SpawnLocation = GetProjectileSpawnLocation();
	if (!bIsAbilityAttack)
	{
		ShootAtLocation(TargetLocation, false, SpawnLocation);
	}

	OutResult.Actor = TargetActor;
	OutResult.Location = TargetLocation;
	OutResult.SpawnOrWeaponLocation = SpawnLocation;
}

void AHRangedWeapon::AttackLocation(const FVector& TargetLocation, bool bIsAbilityAttack, FAttackResult& OutResult)
{
	FVector SpawnLocation = GetProjectileSpawnLocation();
	if (!bIsAbilityAttack)
	{
		ShootAtLocation(TargetLocation, true, SpawnLocation);
	}

	OutResult.Location = TargetLocation;
	OutResult.SpawnOrWeaponLocation = SpawnLocation;
}