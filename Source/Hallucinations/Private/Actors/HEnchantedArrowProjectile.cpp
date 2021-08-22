// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/HEnchantedArrowProjectile.h"
#include "Particles/ParticleSystemComponent.h"

AHEnchantedArrowProjectile::AHEnchantedArrowProjectile()
{
	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailEffect"));
	TrailEffect->SetupAttachment(RootComponent);
}
