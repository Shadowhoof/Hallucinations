// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HAbilityActorInterface.h"

void IHAbilityActorInterface::Initialize(UHAbility* AbilityDesc, AActor* SourceActor)
{
}

void IHAbilityActorInterface::Initialize(UHAbility* AbilityDesc, AActor* SourceActor, AActor* TargetActor)
{
}

bool IHAbilityActorInterface::CanAffectAllies()
{
	return true;
}

bool IHAbilityActorInterface::CanAffectEnemies()
{
	return true;
}
