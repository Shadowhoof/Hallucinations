// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "HSpellAbility.h"
#include "HHeadBlow.generated.h"

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API UHHeadBlow : public UHSpellAbility
{
	GENERATED_BODY()

	friend class AHHeadBlowActor;
	
public:

	UHHeadBlow();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	float Duration;

	virtual void FinishActorCast(AActor* TargetActor) override;
	
};
