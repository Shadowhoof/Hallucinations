// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/HProjectile.h"

#include "HConstants.h"
#include "Components/HHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/HProjectileMovementComponent.h"

// Sets default values
AHProjectile::AHProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Block);
	StaticMesh->SetNotifyRigidBodyCollision(true);
	StaticMesh->IgnoreActorWhenMoving(GetInstigator(), true);
	RootComponent = StaticMesh;

	MaxRange = 10000.f;
	DistanceTraveled = 0.f;
	
	MovementComponent = CreateDefaultSubobject<UHProjectileMovementComponent>(TEXT("MovementComponent"));

}

// Called when the game starts or when spawned
void AHProjectile::BeginPlay()
{
	Super::BeginPlay();

	StaticMesh->OnComponentHit.AddDynamic(this, &AHProjectile::OnHit);
}

void AHProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != GetOwner() && OtherActor != GetInstigator())
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, Data.Damage, -Hit.ImpactNormal, Hit, GetInstigatorController(), this, Data.DamageType);
		GetWorld()->DestroyActor(this);
	}
}

// Called every frame
void AHProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DistanceTraveled += MovementComponent->Velocity.Size() * DeltaTime;
	if (DistanceTraveled > MaxRange)
	{
		GetWorld()->DestroyActor(this);
	}
}

UStaticMeshComponent* AHProjectile::GetMesh() const
{
	return StaticMesh;
}

