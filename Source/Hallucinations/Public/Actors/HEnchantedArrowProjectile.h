// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/HProjectile.h"
#include "HEnchantedArrowProjectile.generated.h"

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API AHEnchantedArrowProjectile : public AHProjectile
{
	GENERATED_BODY()

public:

	AHEnchantedArrowProjectile();
	
protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "VFX")
	UParticleSystemComponent* TrailEffect;
	
};
