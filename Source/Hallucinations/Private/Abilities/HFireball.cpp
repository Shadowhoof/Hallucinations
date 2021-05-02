// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HFireball.h"
#include "Abilities/HFireballImpl.h"
#include "Abilities/HAbilityComponent.h"
#include "Characters/HCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Core/HLogCategories.h"

UHFireball::UHFireball()
{
}

void UHFireball::Use(UHAbilityComponent* Context, AActor* TargetActor)
{
	Super::Use(Context, TargetActor);
	
	ensure(TargetActor);
	CreateProjectile(Context, TargetActor->GetActorLocation());
}

void UHFireball::Use(UHAbilityComponent* Context, FVector TargetLocation)
{
	Super::Use(Context, TargetLocation);
	
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

	FVector SpawnLocation = Caster->GetPawnViewLocation();
	TargetLocation.Z = SpawnLocation.Z;
	FRotator SpawnRotation = (TargetLocation - SpawnLocation).Rotation();
	
	AHFireballImpl* Fireball = Cast<AHFireballImpl>(Caster->GetWorld()->SpawnActor(FireballClass, &SpawnLocation, &SpawnRotation, SpawnParams));
	Fireball->Initialize(*this);
}
