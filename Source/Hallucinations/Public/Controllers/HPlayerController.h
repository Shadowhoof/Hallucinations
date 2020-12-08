// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API AHPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AHPlayerController();

	virtual void PlayerTick(float DeltaSeconds) override;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Target")
	FHitResult MouseoverData;

};
