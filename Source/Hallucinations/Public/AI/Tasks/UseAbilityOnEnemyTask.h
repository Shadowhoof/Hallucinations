// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UseAbilityOnEnemyTask.generated.h"

class UHAbility;
class UHAbilityComponent;

/**
 *  Task that uses any of the available abilities on enemy target
 */
UCLASS()
class HALLUCINATIONS_API UUseAbilityOnEnemyTask : public UBTTaskNode
{
	GENERATED_BODY()

public:
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

	void OnAbilityFinished();
	void OnAbilityCancelled(const UHAbility* Ability);

	TObjectPtr<UBehaviorTreeComponent> TreeComponent;
	
	TWeakObjectPtr<UHAbilityComponent> AbilityComponent;
	
};
