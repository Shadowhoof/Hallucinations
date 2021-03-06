// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapons/HRangedWeapon.h"
#include "HConstants.h"
#include "Actors/HProjectile.h"
#include "Characters/HCharacter.h"

AHRangedWeapon::AHRangedWeapon()
{
	ProjectileClass = AHProjectile::StaticClass();
	ProjectileSocketName = FHConstants::Arrow_Socket_Name;
}

void AHRangedWeapon::ShootAtLocation(const FVector& Location, bool MatchSocketHeight)
{
	AHCharacter* Character = Cast<AHCharacter>(GetOwner());
	FVector SpawnLocation = ProjectileSocketName.IsNone() ? GetTargetLocation() : Character->GetMesh()->GetSocketLocation(ProjectileSocketName);
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
		FHProjectileData ProjectileData;
		ProjectileData.Damage = Damage;
		ProjectileData.DamageType = DamageType;
		Projectile->Data = ProjectileData;
	}
}

void AHRangedWeapon::AttackActor(AActor* const TargetActor)
{
	ShootAtLocation(TargetActor->GetTargetLocation(GetOwner()), false);
}

void AHRangedWeapon::AttackLocation(const FVector& TargetLocation)
{
	ShootAtLocation(TargetLocation, true);
}