// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HFlamestrikeActor.h"

#include "Abilities/HAbilityStatics.h"
#include "Abilities/HFlamestrike.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

AHFlamestrikeActor::AHFlamestrikeActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHFlamestrikeActor::Initialize(UHAbility* AbilityDesc)
{
	UHFlamestrike* FlamestrikeDesc = Cast<UHFlamestrike>(AbilityDesc);
	Damage = FlamestrikeDesc->Damage;
	Radius = FlamestrikeDesc->Radius;
}

void AHFlamestrikeActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	UHAbilityStatics::AreaOfEffectDamage(GetActorLocation(), Damage, Radius, this, GetInstigatorController(),
										 UDamageType::StaticClass());

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionFX, GetActorLocation());

	Destroy();
}
