// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/HSphereProjectile.h"
#include "HConstants.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"

const FName AHSphereProjectile::TrailSizeParameterName = TEXT("Size");

AHSphereProjectile::AHSphereProjectile()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Projectile, ECR_Ignore);
	SphereComponent->SetCollisionObjectType(ECC_Projectile);
	SphereComponent->SetNotifyRigidBodyCollision(true);
	SphereComponent->IgnoreActorWhenMoving(GetInstigator(), true);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AHSphereProjectile::OnBeginOverlap);
	
	RootComponent = SphereComponent;

	TrailEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TrailEffect"));
	TrailEffect->SetupAttachment(RootComponent);
	TrailEffect->SetUsingAbsoluteScale(true);
}

void AHSphereProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	const float Diameter = SphereComponent->GetScaledSphereRadius() * 2.f;
	const float RandomVariation = Diameter * TrailSizeVariation;
	TrailEffect->SetVectorRandParameter(TrailSizeParameterName, FVector(Diameter + RandomVariation), FVector(Diameter - RandomVariation));
}

void AHSphereProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}
