// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffects/HStunEffect.h"
#include "Utils/HEnumTools.h"

void UHStunEffect::Begin(AActor* Affected, AActor* Source, float Duration)
{
	Super::Begin(Affected, Source, Duration);
	
	GetComponent()->ApplyCondition(EStatusCondition::Stunned);
}

void UHStunEffect::End()
{
	Super::End();
	
	GetComponent()->RemoveCondition(EStatusCondition::Stunned);
}

uint8 UHStunEffect::GetActiveConditions() const
{
	EStatusCondition Condition = bIsActive ? EStatusCondition::Stunned : EStatusCondition::None;
	return BIT_AS_INT(Condition);
}
