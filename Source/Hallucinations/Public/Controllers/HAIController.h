// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "HAIController.generated.h"


class UAISense;


/**
 * AI Controller of Hallucinations AI Character
 */
UCLASS()
class HALLUCINATIONS_API AHAIController : public AAIController
{
	GENERATED_BODY()

public:

	AHAIController();

	void GetCurrentlySightPerceivedActors(TArray<AActor*>& OutActors);

	void GetKnownSightPerceivedActors(TArray<AActor*>& OutActors);

	virtual void Tick(float DeltaSeconds) override;

	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Combat")
	bool bInCombat = false;

};
