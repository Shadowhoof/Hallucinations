// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "AbilityEffect.h"
#include "UObject/Object.h"
#include "DamageAbilityEffect.generated.h"

/**
 *	Damage effect
 */
UCLASS()
class HALLUCINATIONS_API UDamageAbilityEffect : public UAbilityEffect
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, Category = "Damage")
	float Damage = 0.f;

	UPROPERTY(EditAnywhere, Category = "Damage")
	TSubclassOf<UDamageType> DamageType = UDamageType::StaticClass();

protected:

	virtual void ApplyToActor(AActor* InInstigatorActor, AController* InInstigatorController, AActor* HitActor, const FHitResult* HitResult) override;
	
};
