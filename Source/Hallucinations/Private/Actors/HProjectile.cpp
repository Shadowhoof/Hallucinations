// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/HProjectile.h"

#include "Kismet/GameplayStatics.h"

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

		const FString OtherActorName = OtherActor ? OtherActor->GetName() : TEXT("Unknown");
		UE_LOG(LogCollision, Verbose, TEXT("Projectile %s hit %s"), *GetName(), *OtherActorName);
	}
}

void AHProjectile::IgnoreActor(AActor* Actor)
{
	StaticMesh->IgnoreActorWhenMoving(Actor, true);
}
