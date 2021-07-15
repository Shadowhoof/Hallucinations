// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HAbilityActorInterface.h"

void IHAbilityActorInterface::Initialize(UHAbility* AbilityDesc)
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
