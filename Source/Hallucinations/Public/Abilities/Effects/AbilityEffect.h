// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "Abilities/HAbilityComponent.h"
#include "UObject/Object.h"
#include "AbilityEffect.generated.h"

/**
 *	Indicates when effect is triggered
 */
UENUM(BlueprintType)
enum class EAbilityEffectTrigger : uint8
{
	Cast,
	Hit
};


/**
 *  
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced, Abstract)
class HALLUCINATIONS_API UAbilityEffect : public UObject
{
	GENERATED_BODY()

public:

	UAbilityEffect();

	/**
	 * @brief Applies current effect either to a location or to an actor.
	 * @param InInstigatorActor Owning actor of an ability effect
	 * @param InInstigatorController Owning controller of an ability effect
	 * @param TargetParams Specifies what exactly was hit by an ability
	 * @param HitResult Hit result
	 */
	virtual void Apply(AActor* InInstigatorActor, AController* InInstigatorController, const FAbilityTargetParameters& TargetParams, const FHitResult* HitResult);
	
protected:

	/** Radius in which effect will be applied. If this is greater than zero then ApplyToLocation will be used even when an actor was hit. */
	UPROPERTY(EditAnywhere, Category = "Ability")
	float Radius = 0.f;

	UPROPERTY(EditAnywhere, Category = "Ability", meta = (Bitmask, BitmaskEnum = "EThreatStatus"))
	uint8 AffectedTargets;

protected:

	/** Applies current effect to a single actor */
	virtual void ApplyToActor(AActor* InInstigatorActor, AController* InInstigatorController, AActor* HitActor, const FHitResult* HitResult);

	/** Applies current effect in an area around provided location */
	virtual void ApplyToLocation(AActor* InInstigatorActor, AController* InInstigatorController, const FVector& HitLocation, const FHitResult* HitResult);
	
};