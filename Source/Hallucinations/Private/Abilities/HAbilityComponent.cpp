// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HAbilityComponent.h"
#include "Abilities/HAbility.h"
#include "Characters/HCharacter.h"
#include "Core/HLogCategories.h"

DEFINE_LOG_CATEGORY(LogAbility);

UHAbilityComponent::UHAbilityComponent()
{
}


bool UHAbilityComponent::CanUseAbility(uint8 Index) const
{
	return Index < Abilities.Num();
}

AActor* UHAbilityComponent::GetTargetActor() const
{
	return GetCaster()->GetTargetActor();
}

FVector UHAbilityComponent::GetTargetLocation() const
{
	return GetCaster()->GetTargetLocation();
}

AHCharacter* UHAbilityComponent::GetCaster() const
{
	return Cast<AHCharacter>(GetOwner());
}

void UHAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (TSubclassOf<UHAbility> AbilityClass : SelectedAbilities)
	{
		UHAbility* Ability = NewObject<UHAbility>(this, AbilityClass);
		Abilities.Add(Ability);
	}
}

void UHAbilityComponent::UseAbility(uint8 Index)
{
	UE_LOG(LogAbility, Log, TEXT("Using ability %d"), Index);
	if (CanUseAbility(Index))
	{
		UE_LOG(LogAbility, Log, TEXT("Can use, success!"));
		Abilities[Index]->TryUse(this);
	}
}
