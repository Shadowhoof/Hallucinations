// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "HAbilityActorInterface.h"
#include "GameFramework/Actor.h"
#include "HHeadBlowActor.generated.h"

UCLASS()
class HALLUCINATIONS_API AHHeadBlowActor : public AActor, public IHAbilityActorInterface
{
	GENERATED_BODY()
	
public:	

	virtual void Initialize(UHAbility* AbilityDesc, AActor* SourceActor, AActor* TargetActor) override;

};
