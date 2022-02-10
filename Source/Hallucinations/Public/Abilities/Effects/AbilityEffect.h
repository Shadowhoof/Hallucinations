// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "Abilities/HAbilityComponent.h"
#include "Constants/HConstants.h"
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


struct FAbilityEffectParameters
{
	/** Actor that used an ability */
	TObjectPtr<AActor> InstigatorActor = nullptr;
	
	/** Controller of an actor that used an ability */
	TObjectPtr<AController> InstigatorController = nullptr;
	
	/** Target actor of an ability */
	TObjectPtr<AActor> TargetActor = nullptr;
	
	/** Target location of an ability */
	FVector TargetLocation = HallucinationsConstants::InvalidVector;
	
	/** Pointer to hit result of an ability. Can be nullptr */
	FHitResult HitResult = FHitResult();
	
	/** Spawn transform. Useful for effects that can spawn actors. */
	FTransform SpawnTransform = FTransform();
};


/**
 *  Effect that will be applied when using an ability. Abilities can have multiple effects and they can be applied
 *  at different times (i.e. when ability is cast or when ability hits)
 */
UCLASS(BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced, Abstract)
class HALLUCINATIONS_API UAbilityEffect : public UObject
{
	GENERATED_BODY()

public:

	UAbilityEffect();

	/**
	 * @brief Applies current effect either to a location or to an actor.
	 * @param Params Parameters that will be used by effect
	 */
	virtual void Apply(const FAbilityEffectParameters& Params);
	
protected:

	/** Radius in which effect will be applied. If this is greater than zero then ApplyToLocation will be used even when an actor was hit. */
	UPROPERTY(EditAnywhere, Category = "Ability")
	float Radius;

	UPROPERTY(EditAnywhere, Category = "Ability", meta = (Bitmask, BitmaskEnum = "EThreatStatus"))
	uint8 AffectedTargets;

protected:

	/** Applies current effect to a single actor */
	virtual void ApplyToActor(AActor* InInstigatorActor, AController* InInstigatorController, AActor* HitActor, const FHitResult& HitResult);

	/** Applies current effect in an area around provided location */
	virtual void ApplyToLocation(AActor* InInstigatorActor, AController* InInstigatorController, const FVector& HitLocation, const FHitResult& HitResult);
	
};