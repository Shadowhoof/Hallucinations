// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HAbstractProjectile.generated.h"

class UHProjectileMovementComponent;

UCLASS()
class HALLUCINATIONS_API AHAbstractProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	
	AHAbstractProjectile();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UHProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float MaxRange = 10000.f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float DistanceTraveled = 0.f;

public:
	
	virtual void Tick(float DeltaTime) override;

	virtual void IgnoreActor(AActor* Actor);

};
