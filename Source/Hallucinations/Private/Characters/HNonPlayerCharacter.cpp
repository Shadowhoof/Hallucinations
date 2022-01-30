// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HNonPlayerCharacter.h"

#include "AIController.h"
#include "Abilities/HAbilityComponent.h"
#include "Components/HHealthComponent.h"
#include "Components/HResourceComponent.h"
#include "Controllers/HAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Core/HLogCategories.h"

AHNonPlayerCharacter::AHNonPlayerCharacter()
{
}



void AHNonPlayerCharacter::EnterCombat()
{
	bInCombat = true;
	GetCharacterMovement()->MaxWalkSpeed = CombatMovementSpeed;
	UE_LOG(LogAI, Verbose, TEXT("%s has entered combat"), *GetName());
}

void AHNonPlayerCharacter::LeaveCombat()
{
	bInCombat = false;
	GetCharacterMovement()->MaxWalkSpeed = NonCombatMovementSpeed;
	AnchorPoint = GetActorLocation();
	UE_LOG(LogAI, Verbose, TEXT("%s has left combat"), *GetName());
}

bool AHNonPlayerCharacter::ShouldBeSaved() const
{
	return !IsDead();
}

FPersistentActorState AHNonPlayerCharacter::GetPersistentState() const
{
	FPersistentActorState State;
	State.Class = GetClass();
	State.Transform = GetTransform();
	State.Health = GetHealthComponent()->GetHealthPercentage();
	State.Mana = GetResourceComponent()->GetManaPercentage();
	AbilityComponent->GetPersistentState(State.AbilityCooldowns);
	return State;
}

void AHNonPlayerCharacter::RestorePersistentState(const FPersistentActorState& State)
{
	HealthComponent->RestorePersistentState(State.Health);
	ResourceComponent->RestorePersistentState(State.Mana);
	AbilityComponent->RestorePersistentState(State.AbilityCooldowns);
}

AActor* AHNonPlayerCharacter::GetTargetActor() const
{
	AHAIController* AIController = Cast<AHAIController>(GetController());
	if (!AIController)
	{
		return nullptr;
	}

	return AIController->GetTargetActor();
}

const FVector& AHNonPlayerCharacter::GetAnchorPoint() const
{
	return AnchorPoint;
}

void AHNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = NonCombatMovementSpeed;

	AnchorPoint = GetActorLocation();
}

void AHNonPlayerCharacter::OnDeath(AActor* Victim, AActor* Killer)
{
	Super::OnDeath(Victim, Killer);

	DetachFromControllerPendingDestroy();
	SetLifeSpan(10.f);
}
