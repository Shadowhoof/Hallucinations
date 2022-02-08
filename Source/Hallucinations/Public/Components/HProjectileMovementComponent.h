// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Constants/HTeamConstants.h"
#include "HProjectileMovementComponent.generated.h"

enum class EThreatStatus : uint8;

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API UHProjectileMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()

public:

	UHProjectileMovementComponent();

	void SetAffectedTargets(EThreatStatus InAffectedTargets);
	
protected:

	virtual EHandleBlockingHitResult HandleBlockingHit(const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining) override;

	virtual void HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta) override;

	/* Targets which will be hit by the projectile */
	EThreatStatus AffectedTargets = EThreatStatus::Enemy;

};
