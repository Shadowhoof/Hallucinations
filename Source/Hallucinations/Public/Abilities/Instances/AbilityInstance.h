// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "Abilities/HAbilityComponent.h"
#include "UObject/Object.h"
#include "AbilityInstance.generated.h"

/**
 *  Unique instance of an ability whenever it is cast
 */
UCLASS(Blueprintable, BlueprintType, DefaultToInstanced, Abstract)
class HALLUCINATIONS_API UAbilityInstance : public UObject
{
	GENERATED_BODY()

public:
	
	UAbilityInstance();

	virtual void Initialize(UHAbility* InAbility, const FAbilityTargetParameters& InTargetParams, AActor* SourceActor, const FTransform& SpawnTransform = FTransform::Identity);

protected:

	UPROPERTY()
	UHAbility* Ability = nullptr;

	FAbilityTargetParameters TargetParams;

	UPROPERTY()
	AActor* InstigatorActor = nullptr;

	UPROPERTY()
	AController* InstigatorController = nullptr;

protected:
	
	void ApplyEffects(const FAbilityTargetParameters& HitParams);

	void ApplyEffectsWithBaseParams();
	
};
