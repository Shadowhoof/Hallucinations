// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/HWeapon.h"

// Sets default values
AHWeapon::AHWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetSimulatePhysics(false);
	StaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	RootComponent = StaticMesh;

	AttachmentSocketName = TEXT("DefaultWeaponSocket");

	WeaponRange = 50.f;
}

// Called when the game starts or when spawned
void AHWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

FName AHWeapon::GetAttachmentSocketName() const
{
	return AttachmentSocketName;
}

bool AHWeapon::IsInRange(AActor* OwnerActor, AActor* TargetActor) const
{
	if (!OwnerActor || !TargetActor)
		return false;
	
	return FVector::Dist(OwnerActor->GetActorLocation(), TargetActor->GetActorLocation()) <= WeaponRange;
}

UAnimMontage* AHWeapon::GetAttackAnimation() const
{
	return AttackAnimation;
}

void AHWeapon::AttackActor(AActor* const TargetActor)
{
}

void AHWeapon::AttackLocation(const FVector& TargetLocation)
{
}

float AHWeapon::GetAttackSpeed() const
{
	return AttackSpeed;
}

