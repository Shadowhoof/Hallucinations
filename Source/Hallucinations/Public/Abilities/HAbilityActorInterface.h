// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HAbilityActorInterface.generated.h"

class UHAbility;

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UHAbilityActorInterface : public UInterface
{
	GENERATED_BODY()
};

class IHAbilityActorInterface
{
	GENERATED_BODY()

public:

	virtual void Initialize(UHAbility* AbilityDesc);

	virtual bool CanAffectAllies();

	virtual bool CanAffectEnemies();
};
