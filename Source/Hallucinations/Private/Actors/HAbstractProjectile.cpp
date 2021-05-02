// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/HAbstractProjectile.h"
#include "Components/HProjectileMovementComponent.h"

// Sets default values
AHAbstractProjectile::AHAbstractProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = CreateDefaultSubobject<UHProjectileMovementComponent>(TEXT("MovementComponent"));
}

// Called every frame
void AHAbstractProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DistanceTraveled += MovementComponent->Velocity.Size() * DeltaTime;
	if (DistanceTraveled > MaxRange)
	{
		GetWorld()->DestroyActor(this);
	}
}

void AHAbstractProjectile::IgnoreActor(AActor* Actor)
{
}

