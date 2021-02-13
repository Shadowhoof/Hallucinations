// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"
#include "HPlayerController.generated.h"

class UCameraComponent;
class AHCharacter;

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API AHPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintImplementableEvent)
	void OnPawnDeath(AHCharacter* Killer, AActor* Victim);
	
public:

	AHPlayerController();

	virtual void PlayerTick(float DeltaSeconds) override;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Target")
	FHitResult MouseoverData;

	virtual void OnPossess(APawn* InPawn) override;
	
	virtual void OnUnPossess() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerRestart();
};
