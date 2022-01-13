// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HFireball.h"

#include "Abilities/HAbilityComponent.h"
#include "Characters/HCharacter.h"
#include "Constants/HConstants.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/HEnumTools.h"
#include "Core/HLogCategories.h"

UHFireball::UHFireball()
{
	TargetType = CombineBits(EAbilityTarget::Actor, EAbilityTarget::Point);
	Name = FText::FromString("Fireball");
	AffectedTargets = BIT_AS_INT(EThreatStatus::Enemy);
}

void UHFireball::FinishActorCast(AActor* TargetActor)
{
	Super::FinishActorCast(TargetActor);
	IHAbilityActorInterface* Projectile = CreateProjectile(TargetActor->GetActorLocation());
	if (!Projectile)
	{
		UE_LOG(LogAbility, Error, TEXT("Failed to spawn fireball projectile actor"));
		return;
	}
	Projectile->Initialize(this, AbilityComponent->GetCaster(), TargetActor);
}

void UHFireball::FinishLocationCast(FVector TargetLocation)
{
	Super::FinishLocationCast(TargetLocation);
	IHAbilityActorInterface* Projectile = CreateProjectile(TargetLocation);
	if (!Projectile)
	{
		UE_LOG(LogAbility, Error, TEXT("Failed to spawn fireball projectile actor"));
		return;
	}
	Projectile->Initialize(this, AbilityComponent->GetCaster());
}

IHAbilityActorInterface* UHFireball::CreateProjectile(FVector TargetLocation)
{
	AHCharacter* Caster = AbilityComponent->GetCaster();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Caster;
	SpawnParams.Instigator = Caster;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = Caster->GetMesh()->GetSocketLocation(FHConstants::SpellSocketName);
	TargetLocation.Z = SpawnLocation.Z;
	FRotator SpawnRotation = (TargetLocation - SpawnLocation).Rotation();

	return CreateActor(Caster->GetWorld(), SpawnLocation, SpawnRotation, SpawnParams);
}
