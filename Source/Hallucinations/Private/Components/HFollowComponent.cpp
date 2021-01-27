// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HFollowComponent.h"
#include "Components/HHealthComponent.h"
#include "HConstants.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Characters/HCharacter.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values for this component's properties
UHFollowComponent::UHFollowComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UHFollowComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UHFollowComponent::ResetRotationFields()
{
	RotationActor = nullptr;
	RotationLocation = FHConstants::Null_Vector;
}

void UHFollowComponent::MoveToActor(AActor* Actor)
{
	if (bIsMovementLocked)
	{
		return;
	}
	
	AHCharacter* Character = Cast<AHCharacter>(GetOwner());
	if (Character)
	{
		UAIBlueprintHelperLibrary::SimpleMoveToActor(Character->Controller, Actor);
	}
	ResetRotationFields();
}

void UHFollowComponent::MoveToLocation(const FVector& Location)
{
	if (bIsMovementLocked)
	{
		return;
	}
	
	AHCharacter* Character = Cast<AHCharacter>(GetOwner());
	if (Character) 
	{
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(Character->Controller, Location);
	}
	ResetRotationFields();
}

void UHFollowComponent::StopMovement()
{
	AHCharacter* Character = Cast<AHCharacter>(GetOwner());
	if (Character) 
	{
		Character->GetMovementComponent()->StopActiveMovement();
	}
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
	StopMovement();
	RotationActor = Actor;
	RotationLocation = FHConstants::Null_Vector;
}

void UHFollowComponent::RotateTowardsLocation(FVector Location)
{
	StopMovement();
	RotationActor = nullptr;
	RotationLocation = Location;
}

void UHFollowComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	const FVector TargetLocation = RotationActor ? RotationActor->GetTargetLocation(GetOwner()) : RotationLocation;
	if (TargetLocation != FHConstants::Null_Vector)
	{
		const FRotator CurrentRotation = GetOwner()->GetActorRotation();
		FRotator TargetRotation = (TargetLocation - GetOwner()->GetActorLocation()).Rotation();
		TargetRotation.Pitch = 0.f;
		if (!CurrentRotation.Equals(TargetRotation))
			GetOwner()->SetActorRotation(FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 10.0));
	}
}

bool UHFollowComponent::CanBeFollowed(AActor* Target)
{
	return Target && Target->GetComponentByClass(UHHealthComponent::StaticClass());
}

