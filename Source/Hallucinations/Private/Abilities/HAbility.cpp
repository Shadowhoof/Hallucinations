// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HAbility.h"

#include "HConstants.h"
#include "Abilities/HAbilityComponent.h"
#include "Components/HFollowComponent.h"
#include "Characters/HCharacter.h"
#include "Utils/HEnumTools.h"
#include "Core/HLogCategories.h"


void UHAbility::Use(UHAbilityComponent* Context, AActor* TargetActor)
{
	UE_LOG(LogAbility, Log, TEXT("Using ability %s on actor %s"), *GetClass()->GetName(), TargetActor ? *TargetActor->GetName() : TEXT("NULL"));
	AHCharacter* Caster = Context->GetCaster();
	UHFollowComponent* FollowComponent = Cast<UHFollowComponent>(Caster->GetComponentByClass(UHFollowComponent::StaticClass()));
	FollowComponent->RotateTowardsActor(TargetActor);
}

void UHAbility::Use(UHAbilityComponent* Context, FVector TargetLocation)
{
	UE_LOG(LogAbility, Log, TEXT("Using ability %s at location %s"), *GetClass()->GetName(), *TargetLocation.ToString());
	AHCharacter* Caster = Context->GetCaster();
	UHFollowComponent* FollowComponent = Cast<UHFollowComponent>(Caster->GetComponentByClass(UHFollowComponent::StaticClass()));
	FollowComponent->RotateTowardsLocation(TargetLocation);
}

void UHAbility::Use(UHAbilityComponent* Context)
{
	UE_LOG(LogAbility, Log, TEXT("Using ability %s on self"), *GetClass()->GetName())
}

bool UHAbility::TryUse(UHAbilityComponent* Context)
{
	const FTimerManager& TimerManager = Context->GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(CooldownTimerHandle))
	{
		// ability is still on cooldown
		UE_LOG(LogAbility, Log, TEXT("Ability is on cooldown, %.2f remaining"), TimerManager.GetTimerRemaining(CooldownTimerHandle));
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("TargetType: %d"), TargetType);
	
	if (TEST_BIT(TargetType, EAbilityTarget::Actor))
	{
		AActor* TargetActor = Context->GetTargetActor();
		if (TargetActor)
		{
			Use(Context, TargetActor);
			return true;
		}
	}

	if (TEST_BIT(TargetType, EAbilityTarget::Point))
	{
		const FVector TargetLocation = Context->GetTargetLocation();
		if (TargetLocation != FHConstants::NullVector)
		{
			Use(Context, TargetLocation);
			return true;
		}
	}

	if (TEST_BIT(TargetType, EAbilityTarget::Self))
	{
		Use(Context);
		return true;
	}

	UE_LOG(LogAbility, Warning, TEXT("No target found for ability %s"), *GetName());
	return false;
}

