// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/HProjectile.h"

#include "HConstants.h"
#include "Components/HHealthComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/HUtils.h"

// Sets default values
AHProjectile::AHProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComponent"));
	RootComponent = SceneRootComponent;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(SceneRootComponent);
	StaticMesh->SetSimulatePhysics(false);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Overlap);
	StaticMesh->SetCollisionResponseToChannel(ECC_Click, ECR_Ignore);

	MaxRange = 10000.f;
	DistanceTraveled = 0.f;
	InitialSpeed = 3000.f;
	
	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->InitialSpeed = InitialSpeed;

	Damage = 10.f;
}

// Called when the game starts or when spawned
void AHProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHProjectile::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor != GetOwner())
	{
		bool bAreEnemies = UHUtils::AreEnemies(GetOwner(), OtherActor);
		UE_LOG(LogTemp, Log, TEXT("Overlapping: %s, isEnemy: %s"), OtherActor ? *OtherActor->GetName() : TEXT("Null"), bAreEnemies ? "true" : "false");
		if (bAreEnemies)
		{
			UGameplayStatics::ApplyDamage(OtherActor, Damage, GetInstigatorController(), this, DamageType);
			GetWorld()->DestroyActor(this);
		}
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

