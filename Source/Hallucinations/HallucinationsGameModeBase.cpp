// Copyright Epic Games, Inc. All Rights Reserved.


#include "HallucinationsGameModeBase.h"

#include "Controllers/HPlayerController.h"

void AHallucinationsGameModeBase::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
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
