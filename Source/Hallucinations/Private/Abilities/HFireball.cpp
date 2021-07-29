// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HFireball.h"

#include "HConstants.h"
#include "Abilities/HAbilityComponent.h"
#include "Characters/HCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/HEnumTools.h"

UHFireball::UHFireball()
{
	TargetType = CombineBits(EAbilityTarget::Actor, EAbilityTarget::Point);
}

void UHFireball::FinishActorCast(UHAbilityComponent* Context, AActor* TargetActor)
{
	Super::FinishActorCast(Context, TargetActor);
	IHAbilityActorInterface* Projectile = CreateProjectile(Context, TargetActor->GetActorLocation());
	Projectile->Initialize(this, Context->GetCaster(), TargetActor);
}

void UHFireball::FinishLocationCast(UHAbilityComponent* Context, FVector TargetLocation)
{
	Super::FinishLocationCast(Context, TargetLocation);
	IHAbilityActorInterface* Projectile = CreateProjectile(Context, TargetLocation);
	Projectile->Initialize(this, Context->GetCaster());
}

IHAbilityActorInterface* UHFireball::CreateProjectile(UHAbilityComponent* Context, FVector TargetLocation)
{
	AHCharacter* Caster = Context->GetCaster();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Caster;
	SpawnParams.Instigator = Caster;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = Caster->GetMesh()->GetSocketLocation(FHConstants::SpellSocketName);
	TargetLocation.Z = SpawnLocation.Z;
	FRotator SpawnRotation = (TargetLocation - SpawnLocation).Rotation();

	return CreateActor(Caster->GetWorld(), SpawnLocation, SpawnRotation, SpawnParams);
}
