// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "HAbilityActorInterface.h"
#include "HFlamestrikeActor.generated.h"

class UParticleSystem;

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API AHFlamestrikeActor : public AActor, public IHAbilityActorInterface
{
	GENERATED_BODY()

public:

	AHFlamestrikeActor();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "FX")
	UParticleSystem* ExplosionFX;
	
	float Damage = 0.f;
	float Radius = 0.f;
	
public:

	virtual void Initialize(UHAbility* AbilityDesc) override;

	virtual void Tick(float DeltaSeconds) override;
	
};
