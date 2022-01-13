// Created by Dmitrii Korovin


#include "AI/Tasks/AttackEnemyTask.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Components/HAttackComponent.h"
#include "Controllers/HAIController.h"

EBTNodeResult::Type UAttackEnemyTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TreeComponent = &OwnerComp;
	
	AHAIController* Controller = Cast<AHAIController>(OwnerComp.GetAIOwner());
	AActor* TargetActor = Controller->GetTargetActor();
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	if (!AttackComponent.IsValid())
	{
		APawn* Pawn = Controller->GetPawn();
		AttackComponent = Cast<UHAttackComponent>(Pawn->GetComponentByClass(UHAttackComponent::StaticClass()));
		AttackComponent->OnAttackEnded.AddDynamic(this, &UAttackEnemyTask::OnAttackEnded);
		AttackComponent->OnAttackInterrupted.AddUObject(this, &UAttackEnemyTask::OnAttackCancelled);
		AttackComponent->OnAttackCancelled.AddUObject(this, &UAttackEnemyTask::OnAttackCancelled);
	}

	if (AttackComponent->AttackActor(TargetActor))
	{
		UE_LOG(LogBehaviorTree, Verbose, TEXT("AI | UAttackEnemyTask | Actor %s is attacking %s"), *AttackComponent->GetOwner()->GetName(), *TargetActor->GetName())
		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UAttackEnemyTask::OnAttackEnded(const FAttackResult& AttackResult)
{
	if (TreeComponent->GetTaskStatus(this) == EBTTaskStatus::Active)
	{
		UE_LOG(LogBehaviorTree, Verbose, TEXT("AI | UAttackEnemyTask has finished with success"));
		FinishLatentTask(*TreeComponent, EBTNodeResult::Succeeded);
		AttackComponent->StopAttacking();
	}
}

void UAttackEnemyTask::OnAttackCancelled()
{
	if (TreeComponent->GetTaskStatus(this) == EBTTaskStatus::Active)
	{
		UE_LOG(LogBehaviorTree, Verbose, TEXT("AI | UAttackEnemyTask has finished with failure"));
		FinishLatentTask(*TreeComponent, EBTNodeResult::Failed);
		AttackComponent->StopAttacking();
	}
}
