// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HCharacter.h"

#include "HConstants.h"
#include "GameFramework/SpringArmComponent.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Controllers/HAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/HHealthComponent.h"
#include "Components/HAttackComponent.h"
#include "Components/HFollowComponent.h"
#include "Components/HAttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Weapons/HWeapon.h"

// Sets default values
AHCharacter::AHCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHHealthComponent>(TEXT("HealthComponent"));
	AttackComponent = CreateDefaultSubobject<UHAttackComponent>(TEXT("AttackComponent"));
	FollowComponent = CreateDefaultSubobject<UHFollowComponent>(TEXT("FollowComponent"));

	GetCapsuleComponent()->SetCapsuleHalfHeight(FHConstants::CapsuleHalfHeight);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;

	AIControllerClass = AHAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AHCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	HealthComponent->OnActorDeath.AddDynamic(this, &AHCharacter::OnDeath);

	AttackComponent->AttackStartedEvent.AddDynamic(this, &AHCharacter::OnAttackStart);
	AttackComponent->AttackEndedEvent.AddDynamic(this, &AHCharacter::OnAttackEnd);
}

void AHCharacter::OnDeath(AActor* Victim, AActor* Killer)
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	UE_LOG(LogDamage, Log, TEXT("%s has died"), *GetName());

	DetachFromControllerPendingDestroy();
	SetLifeSpan(10.f);
	
	DeathEvent.Broadcast(this, Killer);
}

void AHCharacter::OnAttackStart()
{
	FollowComponent->LockMovement();
}

void AHCharacter::OnAttackEnd()
{
	FollowComponent->UnlockMovement();
}

// Called every frame
void AHCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AHCharacter::IsDead() const
{
	return HealthComponent->IsDead();
}

FVector AHCharacter::GetTargetLocation(AActor* RequestedBy) const
{
	const FVector Location = GetActorLocation();
	return FVector(Location.X, Location.Y, Location.Z + GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
}

float AHCharacter::GetCurrentDamage() const
{
	return FMath::RandRange(MinDamage, MaxDamage);
}

float AHCharacter::GetAttackSpeed() const
{
	return AttackSpeed;
}

AActor* AHCharacter::GetTargetActor() const
{
	return nullptr;
}

FVector AHCharacter::GetTargetLocation() const
{
	return FHConstants::NullVector;
}