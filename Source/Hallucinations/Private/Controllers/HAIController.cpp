// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/HAIController.h"
#include "Perception/AIPerceptionComponent.h"

AHAIController::AHAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}
