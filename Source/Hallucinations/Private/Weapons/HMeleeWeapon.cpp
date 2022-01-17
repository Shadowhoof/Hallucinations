// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/HMeleeWeapon.h"
#include "Characters/HCharacter.h"
#include "Components/HAttackComponent.h"
#include "Kismet/GameplayStatics.h"

const float AHMeleeWeapon::RangeLeewayMultiplier = 1.5f;

AHMeleeWeapon::AHMeleeWeapon()
{
	AttackParameters.AttackRange = 200.f;
}

void AHMeleeWeapon::BeginPlay()
{
	Super::BeginPlay();

	RangeWithLeeway = AttackParameters.AttackRange * RangeLeewayMultiplier;
}

void AHMeleeWeapon::AttackActor(AActor* TargetActor, bool bIsAbilityAttack, FAttackResult& OutResult)
{
	bool bIsInRange = TargetActor->GetDistanceTo(GetOwner()) <= RangeWithLeeway;
	if (!bIsAbilityAttack && bIsInRange)
	{
		AHCharacter* Character = Cast<AHCharacter>(GetOwner());
		const float Damage = Character->GetCurrentDamage();

		const FVector ToTargetDirection = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FHitResult HitResult;
		HitResult.HitObjectHandle = FActorInstanceHandle(TargetActor);
		HitResult.ImpactPoint = TargetActor->GetActorLocation();
		HitResult.ImpactNormal = -ToTargetDirection;
		UGameplayStatics::ApplyPointDamage(TargetActor, Damage, ToTargetDirection, HitResult, GetInstigatorController(), this, DamageType);
	}

	if (bIsInRange)
	{
		OutResult.Actor = TargetActor;
		OutResult.Location = TargetActor->GetActorLocation();
	}
	else
	{
		FVector ToTargetDir = (TargetActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		OutResult.Location = GetActorLocation() + ToTargetDir * RangeWithLeeway;
	}
	OutResult.SpawnOrWeaponLocation = GetActorLocation();
}

void AHMeleeWeapon::AttackLocation(const FVector& TargetLocation, bool bIsAbilityAttack,
	FAttackResult& OutResult)
{
	OutResult.Location = TargetLocation;
}
