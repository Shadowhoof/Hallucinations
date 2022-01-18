// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/HAttackComponent.h"
#include "AttackEnemyTask.generated.h"

class UHAttackComponent;

/**
 *  Tasks that initiates attacks on an enemy
 */
UCLASS()
class HALLUCINATIONS_API UAttackEnemyTask : public UBTTaskNode
{
	GENERATED_BODY()

public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	
	void OnAttackEnded();
	
	void OnAttackCancelled();

	TObjectPtr<UBehaviorTreeComponent> TreeComponent;
	
	TWeakObjectPtr<UHAttackComponent> AttackComponent;

	bool bIsAttacking = false;

};
