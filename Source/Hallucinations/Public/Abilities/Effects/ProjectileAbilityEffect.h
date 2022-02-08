// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Effects/AbilityEffect.h"
#include "ProjectileAbilityEffect.generated.h"

class AHAbstractProjectile;


/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API UProjectileAbilityEffect : public UAbilityEffect
{
	GENERATED_BODY()

public:
	
	virtual void Apply(AActor* InInstigatorActor, AController* InInstigatorController, const FAbilityTargetParameters& TargetParams, const FHitResult* HitResult) override;

protected:

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<AHAbstractProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float Speed = 1000.f;
	
	UPROPERTY(EditAnywhere, Instanced, Category = "Projectile")
	TArray<UAbilityEffect*> Effects;

	TObjectPtr<AActor> InstigatorActor;
	TObjectPtr<AController> InstigatorController;
	
protected:
	
	void OnSuccessfulCollision(AActor* HitActor, const FVector& HitLocation, const FHitResult& HitResult);
	
private:

	TObjectPtr<AHAbstractProjectile> Projectile;
	
};
