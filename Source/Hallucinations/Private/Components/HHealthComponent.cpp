// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HHealthComponent.h"

// Sets default values for this component's properties
UHHealthComponent::UHHealthComponent()
{
	MaxHealth = 100.f;
	CurrentHealth = 100.f;
}


// Called when the game starts
void UHHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}


