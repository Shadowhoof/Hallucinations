// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "IdlePerceptionUpdate.generated.h"

class UAISense;

/**
 * Service that runs while AI is in idle state and looks for potential targets or direction to pay attention to.
 */
UCLASS()
class HALLUCINATIONS_API UIdlePerceptionUpdate : public UBTService
{
	GENERATED_BODY()

public:

	UIdlePerceptionUpdate();
	
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blackboard")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Blackboard")
	FBlackboardKeySelector LocationOfInterestKey;

private:

	AActor* GetClosestVisuallyPerceivedEnemy(AAIController* OwnerController, APawn* OwnerPawn) const;

	FVector GetLocationToLookAt(AAIController* OwnerController, APawn* OwnerPawn) const;
	
	FVector GetFreshestSourceLocation(TSubclassOf<UAISense> SenseToUse, AAIController* OwnerController) const;
};
