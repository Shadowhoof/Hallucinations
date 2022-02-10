// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/HAbstractProjectile.h"

#include "Characters/HCharacter.h"
#include "Components/HProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Constants/HTeamConstants.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/HUtils.h"

AHAbstractProjectile::AHAbstractProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	MovementComponent = CreateDefaultSubobject<UHProjectileMovementComponent>(TEXT("MovementComponent"));

	TrailFXComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailEffect"));
	TrailFXComponent->SetupAttachment(RootComponent);
	TrailFXComponent->SetUsingAbsoluteScale(true);

	AffectedTargets = static_cast<uint8>(EThreatStatus::Enemy);
}

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

void AHAbstractProjectile::Initialize(float Speed, EThreatStatus InAffectedTargets)
{
	if (Speed > 0.f)
	{
		MovementComponent->InitialSpeed = Speed;
	}
	AffectedTargets = static_cast<uint8>(InAffectedTargets);
}

bool AHAbstractProjectile::CanCollide(const AActor* ProjectileInstigator, const AActor* HitActor,
	EThreatStatus AffectedProjectileTargets)
{
	if (!ProjectileInstigator->IsA(AHCharacter::StaticClass()) || !HitActor->IsA(AHCharacter::StaticClass()))
	{
		return true;
	}

	return UHUtils::DoesSatisfyThreatStatus(ProjectileInstigator, HitActor, AffectedProjectileTargets);
}

void AHAbstractProjectile::BeginPlay()
{
	Super::BeginPlay();

	IgnoreActor(GetOwner());
	IgnoreActor(GetInstigator());
	MovementComponent->SetAffectedTargets(static_cast<EThreatStatus>(AffectedTargets));
}

void AHAbstractProjectile::OnSuccessfulCollision(AActor* HitActor, const FHitResult& HitResult)
{
	SpawnImpactFX(HitResult.Location);
	SuccessfulCollisionEvent.Broadcast(HitActor, HitResult.Location, HitResult);
}

void AHAbstractProjectile::SpawnImpactFX(const FVector& Location)
{
	if (ImpactFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, Location);
	}
}

