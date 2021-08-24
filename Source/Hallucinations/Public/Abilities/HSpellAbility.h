// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/HAbility.h"
#include "HSpellAbility.generated.h"

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API UHSpellAbility : public UHAbility
{
	GENERATED_BODY()

public:

	UHSpellAbility();

	UAnimMontage* GetCastAnimation() const;

	/** Called by AbilityComponent when cast finishes */
	void OnCastFinished(const FAbilityTargetParameters& TargetParams);
	
protected:

	/** Custom cast animation for this spell */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* CastAnimation;

	/** Class of an actor spawned whenever a spell cast finishes which will perform gameplay actions */
	UPROPERTY(EditDefaultsOnly, Category = "Ability", Meta = (MustImplement = "HAbilityActorInterface"))
	TSubclassOf<AActor> ImplementationClass;

	virtual void FinishActorCast(AActor* TargetActor);
	virtual void FinishLocationCast(FVector TargetLocation);
	virtual void FinishSelfCast();

	/** Creates actor in world that will perform all the gameplay logic */
	virtual IHAbilityActorInterface* CreateActor(UWorld* World, FVector& Location, FRotator& Rotator,
												 FActorSpawnParameters& SpawnParams);
	
};
