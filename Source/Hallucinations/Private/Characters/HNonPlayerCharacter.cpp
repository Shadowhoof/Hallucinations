// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HNonPlayerCharacter.h"

#include "Components/HHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AHNonPlayerCharacter::AHNonPlayerCharacter()
{
}



void AHNonPlayerCharacter::EnterCombat()
{
	bInCombat = true;
	GetCharacterMovement()->MaxWalkSpeed = CombatMovementSpeed;
	UE_LOG(LogTemp, Log, TEXT("%s has entered combat"), *GetName())
}

void AHNonPlayerCharacter::LeaveCombat()
{
	bInCombat = false;
	GetCharacterMovement()->MaxWalkSpeed = NonCombatMovementSpeed;
	UE_LOG(LogTemp, Log, TEXT("%s has left combat"), *GetName())
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
	return State;
}

void AHNonPlayerCharacter::RestorePersistentState(const FPersistentActorState& State)
{
	HealthComponent->RestorePersistentState(State);
}

void AHNonPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->MaxWalkSpeed = NonCombatMovementSpeed;
}
