// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffects/HConditionEffect.h"

#include "Utils/HEnumTools.h"

UHConditionEffect::UHConditionEffect()
{
	Condition = EStatusCondition::None;
}

void UHConditionEffect::Initialize(EStatusCondition StatusCondition)
{
	ensure(Condition == EStatusCondition::None);
	Condition = StatusCondition;
}

void UHConditionEffect::Begin(AActor* Affected, AActor* Source, float Duration)
{
	Super::Begin(Affected, Source, Duration);

	if (Condition != EStatusCondition::None)
	{
		GetComponent()->ApplyCondition(Condition);
	}
}

void UHConditionEffect::End()
{
	Super::End();

	if (Condition != EStatusCondition::None)
	{
		GetComponent()->RemoveCondition(Condition);
	}
}

uint8 UHConditionEffect::GetActiveConditions() const
{
	EStatusCondition CurrentCondition = bIsActive ? Condition : EStatusCondition::None;
	return BIT_AS_INT(CurrentCondition);
}
