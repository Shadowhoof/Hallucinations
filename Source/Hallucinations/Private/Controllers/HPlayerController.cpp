// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/HPlayerController.h"
#include "HConstants.h"

AHPlayerController::AHPlayerController()
{
	CurrentClickTraceChannel = ECC_Click;
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
