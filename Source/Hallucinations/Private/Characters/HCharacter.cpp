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
#include "StatusEffects/HStatusEffectComponent.h"
#include "Components/CapsuleComponent.h"
#include "Abilities/HAbilityComponent.h"
#include "Weapons/HWeapon.h"

// Sets default values
AHCharacter::AHCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// initialize components
	HealthComponent = CreateDefaultSubobject<UHHealthComponent>(TEXT("HealthComponent"));
	AttackComponent = CreateDefaultSubobject<UHAttackComponent>(TEXT("AttackComponent"));
	FollowComponent = CreateDefaultSubobject<UHFollowComponent>(TEXT("FollowComponent"));
	AbilityComponent = CreateDefaultSubobject<UHAbilityComponent>(TEXT("AbilityComponent"));
	StatusEffectComponent = CreateDefaultSubobject<UHStatusEffectComponent>(TEXT("StatusEffectComponent"));

	// subscribe to events
	StatusEffectComponent->OnConditionApplied().AddUObject(this, &AHCharacter::OnConditionApplied);
	StatusEffectComponent->OnConditionRemoved().AddUObject(this, &AHCharacter::OnConditionRemoved);
	
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

void AHCharacter::OnConditionApplied(EStatusCondition Condition)
{
	if (Condition == EStatusCondition::Stunned)
	{
		AttackComponent->StopAttacking(true);
		FollowComponent->Interrupt();
		AbilityComponent->Interrupt();
		GetCharacterMovement()->DisableMovement();
	}
}

void AHCharacter::OnConditionRemoved(EStatusCondition Condition)
{
	if (Condition == EStatusCondition::Stunned)
	{
		UE_LOG(LogTemp, Log, TEXT("Setting default movement mode"));
		GetCharacterMovement()->SetDefaultMovementMode();
	}
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

UHHealthComponent* AHCharacter::GetHealthComponent() const
{
	return HealthComponent;
}

UHAttackComponent* AHCharacter::GetAttackComponent() const
{
	return AttackComponent;
}

UHFollowComponent* AHCharacter::GetFollowComponent() const
{
	return FollowComponent;
}

UHStatusEffectComponent* AHCharacter::GetStatusEffectComponent() const
{
	return StatusEffectComponent;
}

UHAbilityComponent* AHCharacter::GetAbilityComponent() const
{
	return AbilityComponent;
}

bool AHCharacter::IsStunned() const
{
	return StatusEffectComponent->IsConditionActive(EStatusCondition::Stunned);
}

bool AHCharacter::IsBusy() const
{
	if (AttackComponent->IsAttacking() || AbilityComponent->IsCasting() || IsStunned())
	{
		return true;
	}
	
	return false;
}

void AHCharacter::UseAbility(uint8 Index)
{
	if (!IsBusy())
	{
		AbilityComponent->UseAbility(Index);
	}
}
