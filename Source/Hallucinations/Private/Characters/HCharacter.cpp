// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Controllers/HAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/HHealthComponent.h"
#include "Components/HAttackComponent.h"
#include "Components/HFollowComponent.h"
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
	if (Controller)
	{
		Controller->UnPossess();
	}
	SetLifeSpan(10.f);
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

