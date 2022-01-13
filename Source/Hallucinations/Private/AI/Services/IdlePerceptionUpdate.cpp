// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/IdlePerceptionUpdate.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Sight.h"
#include "Constants/HConstants.h"
#include "Utils/HLogUtils.h"
#include "VisualLogger/VisualLogger.h"

UIdlePerceptionUpdate::UIdlePerceptionUpdate()
{
	bNotifyCeaseRelevant = true;
}

void UIdlePerceptionUpdate::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIController* Controller = OwnerComp.GetAIOwner();
	APawn* Pawn = Controller->GetPawn();
	if (!Pawn)
	{
		return;
	}

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	
	// check if we can see a target directly
	AActor* VisibleEnemy = GetClosestVisuallyPerceivedEnemy(Controller, Pawn);
	if (VisibleEnemy)
	{
		BlackboardComponent->SetValueAsObject(TargetActorKey.SelectedKeyName, VisibleEnemy);
		return;
	}

	// check if we have been damaged and turn to that direction
	FVector LastDamageDirection = GetLastDamageSourceLocation(Controller, Pawn);
	if (LastDamageDirection != HallucinationsConstants::InvalidVector)
	{
		BlackboardComponent->SetValueAsVector(LastDamageSourceLocationKey.SelectedKeyName, LastDamageDirection);
	}
	else
	{
		BlackboardComponent->ClearValue(LastDamageSourceLocationKey.SelectedKeyName);
	}

	UE_VLOG(Controller, LogBehaviorTree, Log, TEXT("Updating idle service..."));
}

void UIdlePerceptionUpdate::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
	
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	BlackboardComponent->ClearValue(LastDamageSourceLocationKey.SelectedKeyName);
}

AActor* UIdlePerceptionUpdate::GetClosestVisuallyPerceivedEnemy(AAIController* OwnerController, APawn* OwnerPawn) const
{
	UAIPerceptionComponent* PerceptionComponent = OwnerController->GetPerceptionComponent();
	TArray<AActor*> SightPerceivedActors;
	PerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), SightPerceivedActors);

	float ClosestDistance = FLT_MAX;
	AActor* ClosestActor = nullptr;
	for (AActor* Actor : SightPerceivedActors)
	{
		float Distance = FVector::Distance(Actor->GetActorLocation(), OwnerPawn->GetActorLocation());
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestActor = Actor;
		}
	}
	
	return ClosestActor;
}

FVector UIdlePerceptionUpdate::GetLastDamageSourceLocation(AAIController* OwnerController, APawn* OwnerPawn) const
{
	TSubclassOf<UAISense> DamageSenseClass = UAISense_Damage::StaticClass();
	FAISenseID SenseID = UAISense::GetSenseID(DamageSenseClass);
	if (!SenseID.IsValid())
	{
		return HallucinationsConstants::InvalidVector;
	}

	UAIPerceptionComponent* PerceptionComponent = OwnerController->GetPerceptionComponent();
	const FActorPerceptionInfo* FreshestSource = PerceptionComponent->GetFreshestTrace(SenseID);
	if (FreshestSource && FreshestSource->IsSenseActive(SenseID))
	{
		return FreshestSource->GetLastStimulusLocation();
	}

	return HallucinationsConstants::InvalidVector;
}
