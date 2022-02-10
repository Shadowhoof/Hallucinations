// Created by Dmitrii Korovin


#include "Abilities/Effects/ConditionAbilityEffect.h"
#include "Abilities/HAbilityStatics.h"
#include "StatusEffects/HConditionEffect.h"

void UConditionAbilityEffect::ApplyToActor(AActor* InInstigatorActor, AController* InInstigatorController, AActor* HitActor,
                                           const FHitResult& HitResult)
{
	Super::ApplyToActor(InInstigatorActor, InInstigatorController, HitActor, HitResult);
	
	if (UHStatusEffectComponent* StatusEffectComponent = Cast<UHStatusEffectComponent>(HitActor->GetComponentByClass(UHStatusEffectComponent::StaticClass())))
	{
		StatusEffectComponent->ApplyEffect(UHAbilityStatics::CreateConditionEffect(Condition, this), InInstigatorActor, Duration);		
	}
}
