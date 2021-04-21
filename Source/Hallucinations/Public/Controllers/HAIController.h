// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HAIController.generated.h"


/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API AHAIController : public AAIController
{
	GENERATED_BODY()

public:

	AHAIController();

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Combat")
	bool bInCombat = false;
};
