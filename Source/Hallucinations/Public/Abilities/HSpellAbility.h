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

	/**
	 *	Spell usage consists of three key points: start of the cast (animation begins), the cast itself
	 *	(spell actor is created) and the end of the cast (animation finishes). CastPoint is the time between
	 *	the start of the cast and the cast itself.
	 */
	float GetCastPoint() const;

	/**
	 *	Spell usage consists of three key points: the start of the cast (animation begins), the cast itself
	 *	(spell actor is created) and the end of the cast (animation finishes). CastBackswing is the time between
	 *	the cast itself and the end of the cast. Cast backswing can be cancelled by giving another action to character.
	 */
	float GetCastBackswing() const;
	
protected:

	/** Custom cast animation for this spell */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* CastAnimation;

	/**
	 *	Time between the start of the cast and the cast itself.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cast")
	float CastPoint;

	/**
	 *	Time between the cast itself and the end of the cast. Cast backswing can be cancelled by giving another
	 *	action to character.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cast")
	float CastBackswing;
	
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
