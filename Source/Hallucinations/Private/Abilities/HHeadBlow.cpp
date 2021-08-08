// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HHeadBlow.h"
#include "Utils/HEnumTools.h"
#include "Characters/HCharacter.h"

UHHeadBlow::UHHeadBlow()
{
	TargetType = BIT_AS_INT(EAbilityTarget::Actor);
	Name = FText::FromString("Head Blow");
}

void UHHeadBlow::FinishActorCast(UHAbilityComponent* Context, AActor* TargetActor)
{
	AHCharacter* Caster = Context->GetCaster();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Caster;
	SpawnParams.Instigator = Caster;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FRotator Rotator;
	FVector Location = TargetActor->GetActorLocation();
	IHAbilityActorInterface* AbilityActor = CreateActor(Context->GetWorld(), Location, Rotator, SpawnParams);
	AbilityActor->Initialize(this, Caster, TargetActor);

	Super::FinishActorCast(Context, TargetActor);
}
