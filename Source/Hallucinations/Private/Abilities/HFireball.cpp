// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HFireball.h"

#include "HConstants.h"
#include "Abilities/HFireballImpl.h"
#include "Abilities/HAbilityComponent.h"
#include "Characters/HCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Core/HLogCategories.h"

UHFireball::UHFireball()
{
}

void UHFireball::FinishActorCast(UHAbilityComponent* Context, AActor* TargetActor)
{
	Super::FinishActorCast(Context, TargetActor);
	CreateProjectile(Context, TargetActor->GetActorLocation());
}

void UHFireball::FinishLocationCast(UHAbilityComponent* Context, FVector TargetLocation)
{
	Super::FinishLocationCast(Context, TargetLocation);
	CreateProjectile(Context, TargetLocation);
}

void UHFireball::CreateProjectile(UHAbilityComponent* Context, FVector TargetLocation)
{
	if (!FireballClass)
	{
		UE_LOG(LogAbility, Warning, TEXT("Couldn't spawn fireball actor because no fireball class was specified"));
		return;
	}

	AHCharacter* Caster = Context->GetCaster();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Caster;
	SpawnParams.Instigator = Caster;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = Caster->GetMesh()->GetSocketLocation(FHConstants::SpellSocketName);
	TargetLocation.Z = SpawnLocation.Z;
	FRotator SpawnRotation = (TargetLocation - SpawnLocation).Rotation();

	AActor* SpawnedActor = Caster->GetWorld()->SpawnActor(FireballClass, &SpawnLocation, &SpawnRotation, SpawnParams);
	AHFireballImpl* Fireball = Cast<AHFireballImpl>(SpawnedActor);
	Fireball->Initialize(*this);
}
