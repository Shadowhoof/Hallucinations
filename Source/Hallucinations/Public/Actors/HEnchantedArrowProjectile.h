// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Abilities/HAbilityActorInterface.h"
#include "Actors/HProjectile.h"
#include "StatusEffects/HStatusEffectComponent.h"

#include "HEnchantedArrowProjectile.generated.h"

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API AHEnchantedArrowProjectile : public AHProjectile, public IHAbilityActorInterface
{
	GENERATED_BODY()

public:

	AHEnchantedArrowProjectile();

	virtual void Initialize(UHAbility* AbilityDesc, AActor* SourceActor) override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	UParticleSystemComponent* TrailEffect;

	float Damage;
	EStatusCondition Condition;
	float ConditionDuration;

	virtual void OnSuccessfulHit(AActor* HitActor, const FHitResult& HitResult) override;
};
