// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "HProjectileMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API UHProjectileMovementComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()

public:

	UHProjectileMovementComponent();

protected:

	virtual EHandleBlockingHitResult HandleBlockingHit(const FHitResult& Hit, float TimeTick, const FVector& MoveDelta, float& SubTickTimeRemaining) override;

	virtual void HandleImpact(const FHitResult& Hit, float TimeSlice, const FVector& MoveDelta) override;

};
