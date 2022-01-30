// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HFollowComponent.h"
#include "Constants/HConstants.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Characters/HCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Core/HInteractable.h"
#include "Characters/HPlayerCharacter.h"
#include "Utils/HLogUtils.h"

// Sets default values for this component's properties
UHFollowComponent::UHFollowComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	RotationLocation = FHConstants::NullVector;
}


// Called when the game starts
void UHFollowComponent::BeginPlay()
{
	Super::BeginPlay();

	GetCharacter()->DeathEvent.AddDynamic(this, &UHFollowComponent::OnOwnerDeath);
}

void UHFollowComponent::StopRotation()
{
	RotationActor = nullptr;
	RotationLocation = FHConstants::NullVector;
}

void UHFollowComponent::OnOwnerDeath(AHCharacter* Victim, AActor* Killer)
{
	StopMovement();
}

bool UHFollowComponent::CanIssueMoveOrder() const
{
	return !bIsMovementLocked && !GetCharacter()->IsBusy();
}

bool UHFollowComponent::CanRotate() const
{
	return !GetCharacter()->IsStunned();
}

void UHFollowComponent::MoveToActor(AActor* Actor)
{
	if (!CanIssueMoveOrder())
	{
		return;
	}
	
	UAIBlueprintHelperLibrary::SimpleMoveToActor(GetCharacter()->Controller, Actor);
	FollowedActor = Actor;
	StopRotation();
}

void UHFollowComponent::MoveToLocation(const FVector& Location)
{
	if (!CanIssueMoveOrder())
	{
		return;
	}
	
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(GetCharacter()->Controller, Location);
	FollowedActor = nullptr;
	StopRotation();
}

void UHFollowComponent::StopMovement()
{
	GetCharacter()->GetMovementComponent()->StopActiveMovement();
	FollowedActor = nullptr;
}

void UHFollowComponent::LockMovement()
{
	bIsMovementLocked = true;
	StopMovement();
}

void UHFollowComponent::UnlockMovement()
{
	bIsMovementLocked = false;
}

void UHFollowComponent::RotateTowardsActor(AActor* const Actor)
{
	if (!CanRotate())
	{
		return;
	}
	
	StopMovement();
	RotationActor = Actor;
	RotationLocation = FHConstants::NullVector;
}

void UHFollowComponent::RotateTowardsLocation(FVector Location)
{
	if (!CanRotate())
	{
		return;
	}
	
	StopMovement();
	RotationActor = nullptr;
	RotationLocation = Location;
}

void UHFollowComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	if (!CanRotate())
	{
		return;
	}
	
	const FVector TargetLocation = RotationActor ? RotationActor->GetTargetLocation(GetOwner()) : RotationLocation;
	if (TargetLocation != FHConstants::NullVector)
	{
		const FRotator CurrentRotation = GetOwner()->GetActorRotation();
		FRotator TargetRotation = (TargetLocation - GetOwner()->GetActorLocation()).Rotation();
		TargetRotation.Pitch = 0.f;
		if (!CurrentRotation.Equals(TargetRotation))
		{
			GetOwner()->SetActorRotation(FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 20.0));
		}
	}

	if (FollowedActor)
	{
		IHInteractable* InteractableActor = Cast<IHInteractable>(FollowedActor);
		if (InteractableActor && InteractableActor->IsInRange(GetCharacter()->GetActorLocation()))
		{
			InteractableActor->InteractWith(GetCharacter());
			FollowedActor = nullptr;
		}
	}
}

bool UHFollowComponent::CanBeFollowed(AActor* Target)
{
	return Target && Target->Implements<UHInteractable>();
}

AHCharacter* UHFollowComponent::GetCharacter() const
{
	return Cast<AHCharacter>(GetOwner());
}

void UHFollowComponent::Interrupt()
{
	StopMovement();
	StopRotation();
}

