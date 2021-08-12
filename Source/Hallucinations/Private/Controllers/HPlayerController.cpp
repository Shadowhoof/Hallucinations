// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/HPlayerController.h"
#include "HConstants.h"
#include "Characters/HPlayerCharacter.h"

#include "Navigation/PathFollowingComponent.h"
#include "GameFramework/PawnMovementComponent.h"

AHPlayerController::AHPlayerController()
{
	CurrentClickTraceChannel = ECC_Click;
}

AHPlayerCharacter* AHPlayerController::GetPlayerCharacter() const
{
	return Cast<AHPlayerCharacter>(GetCharacter());
}

void AHPlayerController::PlayerTick(float DeltaSeconds)
{	
	FHitResult HitResult;
	float ScreenSpaceX, ScreenSpaceY;
	GetMousePosition(ScreenSpaceX, ScreenSpaceY);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	GetHitResultAtScreenPosition(FVector2D(ScreenSpaceX, ScreenSpaceY), CurrentClickTraceChannel, QueryParams, HitResult);

	MouseoverData = HitResult;

	Super::PlayerTick(DeltaSeconds);
}

void AHPlayerController::OnPossess(APawn* InPawn)
{
	if (AHPlayerCharacter* PlayerCharacter = Cast<AHPlayerCharacter>(InPawn); PlayerCharacter)
	{
		PlayerCharacter->DeathEvent.AddDynamic(this, &AHPlayerController::OnPawnDeath);
	}

	Super::OnPossess(InPawn);

	// update cached movement component after player restart
	UPathFollowingComponent* PathFollowingComp = Cast<UPathFollowingComponent>(FindComponentByClass(UPathFollowingComponent::StaticClass()));
	if (PathFollowingComp) 
	{
		PathFollowingComp->UpdateCachedComponents();
	}
}

void AHPlayerController::OnUnPossess()
{
	if (GetPawn()) 
	{
		ControlRotation = PlayerCameraManager->GetCameraRotation();
	}
	
	Super::OnUnPossess();
}


