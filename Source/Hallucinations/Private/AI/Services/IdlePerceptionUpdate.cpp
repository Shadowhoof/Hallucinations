// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/IdlePerceptionUpdate.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Damage.h"
#include "Perception/AISense_Sight.h"
#include "Constants/HConstants.h"
#include "Perception/AISense_Team.h"
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
	const FVector LocationToLookAt = GetLocationToLookAt(Controller, Pawn);
	if (LocationToLookAt != HallucinationsConstants::InvalidVector)
	{
		BlackboardComponent->SetValueAsVector(LocationOfInterestKey.SelectedKeyName, LocationToLookAt);
	}
	else
	{
		BlackboardComponent->ClearValue(LocationOfInterestKey.SelectedKeyName);
	}
}

void UIdlePerceptionUpdate::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
	
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	BlackboardComponent->ClearValue(LocationOfInterestKey.SelectedKeyName);
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

FVector UIdlePerceptionUpdate::GetLocationToLookAt(AAIController* OwnerController, APawn* OwnerPawn) const
{
	static const TArray<TSubclassOf<UAISense>> SensesToUse = {UAISense_Damage::StaticClass(), UAISense_Team::StaticClass()};
	for (const TSubclassOf<UAISense>& SenseClass : SensesToUse)
	{
		if (const FVector Location = GetFreshestSourceLocation(SenseClass, OwnerController); Location != HallucinationsConstants::InvalidVector)
		{
			return Location;
		}
	}

	return HallucinationsConstants::InvalidVector;
}

FVector UIdlePerceptionUpdate::GetFreshestSourceLocation(TSubclassOf<UAISense> SenseToUse, AAIController* OwnerController) const
{
	if (const FAISenseID SenseID = UAISense::GetSenseID(SenseToUse); SenseID.IsValid())
	{
		const FActorPerceptionInfo* FreshestSource = OwnerController->GetPerceptionComponent()->GetFreshestTrace(SenseID);
		if (FreshestSource && FreshestSource->IsSenseActive(SenseID))
		{
			return FreshestSource->GetLastStimulusLocation();
		}
	}

	return HallucinationsConstants::InvalidVector;
}
