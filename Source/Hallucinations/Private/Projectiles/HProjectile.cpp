// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectiles/HProjectile.h"

#include "Constants/HConstants.h"
#include "Characters/HCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/HLogUtils.h"
#include "Utils/HUtils.h"

// Sets default values
AHProjectile::AHProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Block);
	StaticMesh->SetCollisionResponseToChannel(ECC_Click, ECR_Ignore);
	StaticMesh->SetCollisionResponseToChannel(ECC_Projectile, ECR_Ignore);
	StaticMesh->SetNotifyRigidBodyCollision(true);
	
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void AHProjectile::BeginPlay()
{
	Super::BeginPlay();

	IgnoreActor(GetInstigator());
	StaticMesh->OnComponentHit.AddDynamic(this, &AHProjectile::OnHit);
}

void AHProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult)
{
	if (CanCollide(GetInstigator(), OtherActor, static_cast<EThreatStatus>(AffectedTargets)))
	{
		OnSuccessfulCollision(OtherActor, HitResult);
		Destroy();
	}
	else
	{
		IgnoreActor(OtherActor);
	}
}

void AHProjectile::OnSuccessfulCollision(AActor* HitActor, const FHitResult& HitResult)
{
	UGameplayStatics::ApplyPointDamage(HitActor, Data.Damage, -HitResult.ImpactNormal, HitResult, GetInstigatorController(), this, Data.DamageType);
	Super::OnSuccessfulCollision(HitActor, HitResult);
}

void AHProjectile::IgnoreActor(AActor* Actor)
{
	if (!Actor)
	{
		return;
	}
	
	StaticMesh->IgnoreActorWhenMoving(Actor, true);
	AHCharacter* AsCharacter = Cast<AHCharacter>(Actor);
	if (AsCharacter)
	{
		AsCharacter->IgnoreActorWhenMoving(this);
	}
}
