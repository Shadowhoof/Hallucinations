// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/HPlayerController.h"
#include "Constants/HConstants.h"
#include "Characters/HPlayerCharacter.h"
#include "Components/HHealthComponent.h"
#include "Camera/CameraActor.h"
#include "Navigation/PathFollowingComponent.h"
#include "Utils/HLogUtils.h"

AHPlayerController::AHPlayerController()
{
	CurrentClickTraceChannel = ECC_Click;
}

AHPlayerCharacter* AHPlayerController::GetPlayerCharacter() const
{
	return Cast<AHPlayerCharacter>(GetCharacter());
}

void AHPlayerController::ProcessMouseover()
{
	FHitResult HitResult;
	float ScreenSpaceX, ScreenSpaceY;
	GetMousePosition(ScreenSpaceX, ScreenSpaceY);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	GetHitResultAtScreenPosition(FVector2D(ScreenSpaceX, ScreenSpaceY), CurrentClickTraceChannel, QueryParams, HitResult);

	MouseoverData = HitResult;
	HandleHoveredActor(MouseoverData.GetActor());
}

void AHPlayerController::HandleHoveredActor(AActor* Actor)
{
	if (HoveredActor == Actor)
	{
		return;
	}

	if (HoveredActor)
	{
		if (IHInteractable* InteractableActor = Cast<IHInteractable>(HoveredActor); InteractableActor)
		{
			InteractableActor->Execute_OnHoverEnd(HoveredActor);
		}
	}

	if (Actor)
	{
		if (IHInteractable* InteractableActor = Cast<IHInteractable>(Actor); InteractableActor)
		{
			InteractableActor->Execute_OnHoverBegin(Actor);
		}
	}

	HoveredActor = Actor;
}

void AHPlayerController::PlayerTick(float DeltaSeconds)
{	
	ProcessMouseover();

	Super::PlayerTick(DeltaSeconds);
}

void AHPlayerController::OnPossess(APawn* InPawn)
{
	AHPlayerCharacter* PlayerCharacter = Cast<AHPlayerCharacter>(InPawn);
	if (PlayerCharacter)
	{
		PlayerCharacter->DeathEvent.AddDynamic(this, &AHPlayerController::OnPawnDeath);
		GenericTeamId = FGenericTeamId((uint8) PlayerCharacter->GetHealthComponent()->GetTeam());
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

FGenericTeamId AHPlayerController::GetGenericTeamId() const
{
	return GenericTeamId;
}

void AHPlayerController::BeginPlay()
{
	Super::BeginPlay();
}


