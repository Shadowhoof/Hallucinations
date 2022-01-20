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
#include "StatusEffects/HStatusEffectComponent.h"
#include "Components/CapsuleComponent.h"
#include "Abilities/HAbilityComponent.h"
#include "Components/BoxComponent.h"
#include "Components/HResourceComponent.h"
#include "Constants/HConstants.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/HLogUtils.h"


namespace CharacterConstants
{
	const float BoxExtraExtent = 50.f;

	// FIXME - this should probably be in movement component
	const float ChilledMoveSpeedMultiplier = 0.5f;
}


// Sets default values
AHCharacter::AHCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// initialize components
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	HealthComponent = CreateDefaultSubobject<UHHealthComponent>(TEXT("HealthComponent"));
	ResourceComponent = CreateDefaultSubobject<UHResourceComponent>(TEXT("ResourceComponent"));
	AttackComponent = CreateDefaultSubobject<UHAttackComponent>(TEXT("AttackComponent"));
	FollowComponent = CreateDefaultSubobject<UHFollowComponent>(TEXT("FollowComponent"));
	AbilityComponent = CreateDefaultSubobject<UHAbilityComponent>(TEXT("AbilityComponent"));
	StatusEffectComponent = CreateDefaultSubobject<UHStatusEffectComponent>(TEXT("StatusEffectComponent"));

	// subscribe to events
	StatusEffectComponent->OnConditionApplied().AddUObject(this, &AHCharacter::OnConditionApplied);
	StatusEffectComponent->OnConditionRemoved().AddUObject(this, &AHCharacter::OnConditionRemoved);

	UCapsuleComponent* Capsule = GetCapsuleComponent();
	Capsule->SetCapsuleHalfHeight(FHConstants::CapsuleHalfHeight);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	float BoxRadius = Capsule->GetUnscaledCapsuleRadius() + CharacterConstants::BoxExtraExtent;
	float BoxHeight = FHConstants::CapsuleHalfHeight + BoxRadius;
	BoxComponent->SetBoxExtent(FVector(BoxRadius, BoxRadius, BoxHeight));
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Click, ECR_Block);
	BoxComponent->SetupAttachment(Capsule);
	
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

	AttackComponent->OnAttackStarted.AddDynamic(this, &AHCharacter::OnAttackStart);
	AttackComponent->OnAttackPointReached.AddDynamic(this, &AHCharacter::OnAttackEnd);
}

void AHCharacter::OnDeath(AActor* Victim, AActor* Killer)
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	
	StatusEffectComponent->OnDeath();

	UE_LOG(LogDamage, Log, TEXT("%s has died"), *GetName());
	DeathEvent.Broadcast(this, Killer);
}

void AHCharacter::OnAttackStart()
{
	FollowComponent->LockMovement();
}

void AHCharacter::OnAttackEnd(const FAttackResult& AttackResult)
{
	FollowComponent->UnlockMovement();
}

void AHCharacter::OnConditionApplied(EStatusCondition Condition)
{
	switch (Condition)
	{
	case EStatusCondition::Stunned:
		AttackComponent->StopAttacking(EStopAttackReason::Interrupt);
		FollowComponent->Interrupt();
		AbilityComponent->Interrupt();
		GetCharacterMovement()->DisableMovement();
		break;
	case EStatusCondition::Chilled:
		GetCharacterMovement()->MaxWalkSpeed *= CharacterConstants::ChilledMoveSpeedMultiplier;
		break;
	default:
		break;
	}
}

void AHCharacter::OnConditionRemoved(EStatusCondition Condition)
{
	switch (Condition)
	{
	case EStatusCondition::Stunned:
		GetCharacterMovement()->SetDefaultMovementMode();
		break;
	case EStatusCondition::Chilled:
		GetCharacterMovement()->MaxWalkSpeed *= 1.f / CharacterConstants::ChilledMoveSpeedMultiplier;
		break; 
	default:
		break;
	}
}

void AHCharacter::UpdateVisibility()
{
	APlayerController* LocalPlayerController = GetWorld()->GetFirstPlayerController();
	APawn* LocalPawn = LocalPlayerController->GetPawn();
	if (!LocalPawn)
	{
		return;
	}
	
	bool bIsInvisible = GetWorld()->LineTraceTestByChannel(GetActorLocation(), LocalPawn->GetActorLocation(), ECC_Visibility);
	if (bIsInvisible != IsHidden())
	{
		SetHidden(bIsInvisible);
		GetMesh()->SetHiddenInGame(bIsInvisible, true);

		ECollisionResponse NewClickCollision = bIsInvisible ? ECR_Ignore : ECR_Block;
		BoxComponent->SetCollisionResponseToChannel(ECC_Click, NewClickCollision);
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

AActor* AHCharacter::GetTargetActor() const
{
	return nullptr;
}

FVector AHCharacter::GetTargetLocation() const
{
	return HallucinationsConstants::InvalidVector;
}

UHHealthComponent* AHCharacter::GetHealthComponent() const
{
	return HealthComponent;
}

UHResourceComponent* AHCharacter::GetResourceComponent() const
{
	return ResourceComponent;
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

void AHCharacter::IgnoreActorWhenMoving(AActor* Actor)
{
	GetCapsuleComponent()->IgnoreActorWhenMoving(Actor, true);
	GetMesh()->IgnoreActorWhenMoving(Actor, true);
}

void AHCharacter::InteractWith(AHCharacter* Interactor)
{
	// do nothing for now, this should be an attack command
}

FVector AHCharacter::GetInteractableLocation() const
{
	return GetActorLocation();
}

const FText& AHCharacter::GetCharacterName() const
{
	return Name;
}

void AHCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateVisibility();
}
