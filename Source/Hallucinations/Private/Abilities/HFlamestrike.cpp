// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HFlamestrike.h"
#include "Characters/HCharacter.h"
#include "Utils/HEnumTools.h"

UHFlamestrike::UHFlamestrike()
{
	TargetType = BIT_AS_INT(EAbilityTarget::Point);
}

void UHFlamestrike::FinishLocationCast(UHAbilityComponent* Context, FVector TargetLocation)
{
	Super::FinishLocationCast(Context, TargetLocation);

	AHCharacter* Caster = Context->GetCaster();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Caster;
	SpawnParams.Instigator = Caster;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FRotator Rotator;
	IHAbilityActorInterface* AbilityActor = CreateActor(Context->GetWorld(), TargetLocation, Rotator, SpawnParams);
	AbilityActor->Initialize(this, Caster);
}
