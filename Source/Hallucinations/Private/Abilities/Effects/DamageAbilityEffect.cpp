// Created by Dmitrii Korovin


#include "Abilities/Effects/DamageAbilityEffect.h"

#include "Abilities/HAbilityStatics.h"

void UDamageAbilityEffect::ApplyToActor(AActor* InInstigatorActor, AController* InInstigatorController, AActor* HitActor,
                                        const FHitResult& HitResult)
{
	Super::ApplyToActor(InInstigatorActor, InInstigatorController, HitActor, HitResult);
	
	UHAbilityStatics::DealSingleTargetDamage(HitActor, Damage, InInstigatorActor, InInstigatorController, DamageType, HitResult);
}
