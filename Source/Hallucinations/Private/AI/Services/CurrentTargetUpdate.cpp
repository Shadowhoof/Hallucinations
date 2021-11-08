// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/CurrentTargetUpdate.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Controllers/HAIController.h"
#include "Core/HLogCategories.h"
#include "Perception/AIPerceptionComponent.h"
#include "Utils/HLogUtils.h"

void UCurrentTargetUpdate::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AHAIController* Controller = Cast<AHAIController>(OwnerComp.GetAIOwner());
	APawn* Pawn = Controller->GetPawn();
	if (!Pawn)
	{
		UE_LOG(LogAI, Error, TEXT("CurrentTargetUpdate service | Controller has no pawn"));
		return;
	}

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (TargetActor)
	{
		TArray<AActor*> CurrentlyVisibleActors;
		Controller->GetCurrentlySightPerceivedActors(CurrentlyVisibleActors);

		bool bIsTargetVisible = CurrentlyVisibleActors.Contains(TargetActor);
		BlackboardComponent->SetValueAsBool(IsTargetVisibleKey.SelectedKeyName, bIsTargetVisible);
		if (bIsTargetVisible)
		{
			BlackboardComponent->SetValueAsVector(LastKnownLocationKey.SelectedKeyName, TargetActor->GetActorLocation());
			return;
		}
	}

	TArray<AActor*> KnownActorsBySight;
	Controller->GetKnownSightPerceivedActors(KnownActorsBySight);
	if (!KnownActorsBySight.Contains(TargetActor))
	{
		BlackboardComponent->ClearValue(TargetActorKey.SelectedKeyName);
		BlackboardComponent->ClearValue(LastKnownLocationKey.SelectedKeyName);
		BlackboardComponent->SetValueAsBool(IsTargetVisibleKey.SelectedKeyName, false);
	}
}
