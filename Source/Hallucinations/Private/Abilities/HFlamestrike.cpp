// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HFlamestrike.h"
#include "Characters/HCharacter.h"
#include "Utils/HEnumTools.h"

UHFlamestrike::UHFlamestrike()
{
	TargetType = BIT_AS_INT(EAbilityTarget::Point);
	Name = FText::FromString("Flamestrike");
}

void UHFlamestrike::FinishLocationCast(FVector TargetLocation)
{
	Super::FinishLocationCast(TargetLocation);

	AHCharacter* Caster = AbilityComponent->GetCaster();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Caster;
	SpawnParams.Instigator = Caster;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FRotator Rotator;
	IHAbilityActorInterface* AbilityActor = CreateActor(AbilityComponent->GetWorld(), TargetLocation, Rotator, SpawnParams);
	AbilityActor->Initialize(this, Caster);
}
