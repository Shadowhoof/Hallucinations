// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CurrentTargetUpdate.generated.h"

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API UCurrentTargetUpdate : public UBTService
{
	GENERATED_BODY()

public:
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blackboard")
	FBlackboardKeySelector LastKnownLocationKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blackboard")
	FBlackboardKeySelector IsTargetVisibleKey;
	
};
