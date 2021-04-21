// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/HGameMode.h"
#include "Characters/HPlayerCharacter.h"
#include "Controllers/HPlayerController.h"


AHGameMode::AHGameMode()
{
	DefaultPawnClass = AHPlayerCharacter::StaticClass();
	PlayerControllerClass = AHPlayerController::StaticClass();
}

void AHGameMode::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	Super::FinishRestartPlayer(NewPlayer, StartRotation);

	if (NewPlayer->GetPawn() != nullptr) 
	{
		AHPlayerController* PlayerController = Cast<AHPlayerController>(NewPlayer);
		if (PlayerController) 
		{
			PlayerController->OnPlayerRestart();
		}
	}
}
