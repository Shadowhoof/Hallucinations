// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/HUtils.h"

#include "Components/HHealthComponent.h"

bool UHUtils::CanBeTargeted(AActor* TargetActor)
{
	return TargetActor && TargetActor->GetComponentByClass(UHHealthComponent::StaticClass());
}

bool UHUtils::CanBeAttacked(AActor* Attacker, AActor* Victim)
{
	return CanBeTargeted(Victim) && AreEnemies(Attacker, Victim);
}

bool UHUtils::AreEnemies(AActor* First, AActor* Second)
{
	return UHHealthComponent::AreEnemies(First, Second);
}

