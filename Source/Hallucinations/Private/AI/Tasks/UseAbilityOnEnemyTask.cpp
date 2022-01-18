// Created by Dmitrii Korovin


#include "AI/Tasks/UseAbilityOnEnemyTask.h"

#include "Abilities/HAbilityComponent.h"
#include "Abilities/HAbility.h"
#include "Constants/HConstants.h"
#include "Controllers/HAIController.h"

EBTNodeResult::Type UUseAbilityOnEnemyTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	TreeComponent = &OwnerComp;
	
	const AHAIController* Controller = Cast<AHAIController>(OwnerComp.GetAIOwner());
	AActor* TargetActor = Controller->GetTargetActor();
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	if (!AbilityComponent.IsValid())
	{
		const APawn* Pawn = Controller->GetPawn();
		AbilityComponent = Cast<UHAbilityComponent>(Pawn->GetComponentByClass(UHAbilityComponent::StaticClass()));
		AbilityComponent->OnCastBackswingFinished.AddUObject(this, &UUseAbilityOnEnemyTask::OnAbilityFinished);
		AbilityComponent->OnAbilityCancelled.AddUObject(this, &UUseAbilityOnEnemyTask::OnAbilityCancelled);
	}
	
	const FAbilityTargetParameters TargetParams{TargetActor, HallucinationsConstants::InvalidVector};
	for (UHAbility* Ability : AbilityComponent->GetAbilities())
	{
		if (Ability->IsOffensive() && Ability->CanBeUsed(TargetParams))
		{
			UE_LOG(LogBehaviorTree, Verbose, TEXT("AI | UUseAbilityOnEnemyTask | Actor %s is using ability %s on %s"),
				*AbilityComponent->GetOwner()->GetName(), *Ability->GetSkillNameAsString(), *TargetActor->GetName())

			if (AbilityComponent->UseAbility(Ability))
			{
				return EBTNodeResult::InProgress;
			}
		}
	}

	return EBTNodeResult::Failed;
}

void UUseAbilityOnEnemyTask::OnAbilityFinished()
{
	if (TreeComponent->GetTaskStatus(this) == EBTTaskStatus::Active)
	{
		UE_LOG(LogBehaviorTree, Verbose, TEXT("AI | UUseAbilityOnEnemyTask has finished with success"));
		FinishLatentTask(*TreeComponent, EBTNodeResult::Succeeded);
	}
}

void UUseAbilityOnEnemyTask::OnAbilityCancelled(const UHAbility* Ability)
{
	if (TreeComponent->GetTaskStatus(this) == EBTTaskStatus::Active)
	{
		UE_LOG(LogBehaviorTree, Verbose, TEXT("AI | UUseAbilityOnEnemyTask has finished with failure"));
		FinishLatentTask(*TreeComponent, EBTNodeResult::Failed);	
	}
}
