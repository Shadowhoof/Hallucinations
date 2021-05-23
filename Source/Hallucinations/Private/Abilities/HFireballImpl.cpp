// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HFireballImpl.h"
#include "DrawDebugHelpers.h"
#include "HConstants.h"
#include "Abilities/HFireball.h"
#include "Components/HHealthComponent.h"
#include "Components/HProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Core/HLogCategories.h"

AHFireballImpl::AHFireballImpl()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Projectile, ECR_Ignore);
	CollisionComponent->SetCollisionObjectType(ECC_Projectile);

	CollisionComponent->SetNotifyRigidBodyCollision(true);
	CollisionComponent->IgnoreActorWhenMoving(GetInstigator(), true);
	//StaticMesh->OnComponentHit.AddDynamic(this, &AHFireballImpl::OnHit);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AHFireballImpl::OnBeginOverlap);

	RootComponent = CollisionComponent;

	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailEffect"));
	TrailEffect->SetupAttachment(RootComponent);
	TrailEffect->SetUsingAbsoluteScale(true);
}

void AHFireballImpl::Initialize(UHFireball& AbilityDesc)
{
	const FVector Velocity = FVector(AbilityDesc.Speed, 0.f, 0.f);
	MovementComponent->SetVelocityInLocalSpace(Velocity);
	MovementComponent->ProjectileGravityScale = 0.f;

	HitDamage = AbilityDesc.HitDamage;
	ExplosionDamage = AbilityDesc.ExplosionDamage;
	ExplosionRadius = AbilityDesc.ExplosionRadius;
	bIsFalloff = AbilityDesc.bIsFalloff;
}

void AHFireballImpl::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != GetOwner() && OtherActor != GetInstigator())
	{
		UE_LOG(LogAbility, Log, TEXT("Fireball hit actor: %s"),
		       OtherActor ? *OtherActor->GetName() : TEXT("Null (BSP?)"));
		Destroy();
	}
}

void AHFireballImpl::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                    const FHitResult& SweepResult)
{
	if (UHHealthComponent::AreAllies(GetInstigator(), OtherActor))
	{
		// fireball should go through allies
		// TODO: move this to base projectile class
		return;
	}

	UE_LOG(LogAbility, Log, TEXT("Fireball impact on %s"), OtherActor ? *OtherActor->GetName() : TEXT("Null (BSP?)"));

	// damage hit target directly
	const FVector HitDirection = SweepResult.TraceEnd - SweepResult.TraceStart;
	UGameplayStatics::ApplyPointDamage(OtherActor, HitDamage, HitDirection, SweepResult,
	                                   GetInstigatorController(), GetInstigator(), UDamageType::StaticClass());

	// damage everyone in AoE
	const TArray<AActor*> IgnoredActors = {GetInstigator()};
	UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, GetActorLocation(), ExplosionRadius,
	                                    UDamageType::StaticClass(), IgnoredActors, GetInstigator(),
	                                    GetInstigatorController(), true);

	Destroy();
}
