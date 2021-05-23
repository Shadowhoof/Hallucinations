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
	return Index < Abilities.Num() && Abilities[Index];
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

	Abilities.Init(nullptr, MaxAbilities);
	ensure(SelectedAbilities.Num() <= MaxAbilities);

	for (uint8 i = 0; i < SelectedAbilities.Num(); i++)
	{
		TSubclassOf<UHAbility> AbilityClass = SelectedAbilities[i];
		if (AbilityClass)
		{
			Abilities[i] = NewObject<UHAbility>(this, AbilityClass);
		}
	}
}

void UHAbilityComponent::UseAbility(uint8 Index)
{
	if (CanUseAbility(Index))
	{
		Abilities[Index]->TryUse(this);
	}
}

void UHAbilityComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.GetPropertyName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UHAbilityComponent, MaxAbilities))
	{
		SelectedAbilities.SetNum(MaxAbilities);
	}
}