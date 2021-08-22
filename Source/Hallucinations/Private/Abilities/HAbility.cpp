// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HAbility.h"

#include "Abilities/HAbilityComponent.h"
#include "Components/HFollowComponent.h"
#include "Characters/HCharacter.h"
#include "Utils/HEnumTools.h"
#include "Core/HLogCategories.h"
#include "TimerManager.h"
#include "Utils/HUtils.h"


float UHAbility::GetRemainingCooldown() const
{	
	UWorld* World = AbilityComponent->GetWorld();
	return World->GetTimerManager().GetTimerRemaining(CooldownTimerHandle);
}

float UHAbility::GetRemainingCooldownPercentage() const
{
	return GetRemainingCooldown() / Cooldown;
}

FText UHAbility::GetSkillName() const
{
	return Name.IsEmpty() ? FText::FromString(GetClass()->GetName()) : Name;
}

const FString& UHAbility::GetSkillNameAsString() const
{
	return GetSkillName().ToString();
}

bool UHAbility::CanBeUsed(const FAbilityTargetParameters& TargetParams) const
{
	FTimerManager& TimerManager = AbilityComponent->GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(CooldownTimerHandle))
	{
		UE_LOG(LogAbility, VeryVerbose, TEXT("Ability is on cooldown, %.2f remaining"), TimerManager.GetTimerRemaining(CooldownTimerHandle));
		return false;
	}

	return IsTargetTypeValid(TargetParams);
}

bool UHAbility::IsTargetTypeValid(const FAbilityTargetParameters& TargetParams) const
{
	return GetTargetType(TargetParams) != EAbilityTarget::None;
}

void UHAbility::StartCooldown()
{
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, Cooldown, false);
}

EAbilityType UHAbility::GetType() const
{
	return Type;
}

EAbilityTarget UHAbility::GetTargetType(const FAbilityTargetParameters& TargetParams) const
{
	if (TEST_BIT(TargetType, EAbilityTarget::Actor) && TargetParams.Actor.IsValid())
	{
		return EAbilityTarget::Actor;
	}
	if (TEST_BIT(TargetType, EAbilityTarget::Point) && UHUtils::IsVectorValid(TargetParams.Location))
	{
		return EAbilityTarget::Point;
	}
	if (TEST_BIT(TargetType, EAbilityTarget::Self))
	{
		return EAbilityTarget::Self;
	}
	return EAbilityTarget::None;
}

UAnimMontage* UHAbility::GetCastAnimation() const
{
	return CastAnimation;
}

void UHAbility::OnCastFinished(const FAbilityTargetParameters& TargetParams)
{
	EAbilityTarget Target = GetTargetType(TargetParams);
	switch (Target)
	{
	case EAbilityTarget::Actor:
		FinishActorCast(TargetParams.Actor.Get());
		break;
	case EAbilityTarget::Point:
		FinishLocationCast(TargetParams.Location);
		break;
	case EAbilityTarget::Self:
		FinishSelfCast();
		break;
	default:
		// cast is unavailable for provided target params, just return and do nothing
		return;
	}

	StartCooldown();
}

void UHAbility::SetAbilityComponent(UHAbilityComponent* Component)
{
	ensure(!AbilityComponent && Component);
	AbilityComponent = Component;
}

void UHAbility::FinishActorCast(AActor* TargetActor)
{
}

void UHAbility::FinishLocationCast(FVector TargetLocation)
{
}

void UHAbility::FinishSelfCast()
{
}

IHAbilityActorInterface* UHAbility::CreateActor(UWorld* World, FVector& Location, FRotator& Rotator,
												FActorSpawnParameters& SpawnParams)
{
	if (!ImplementationClass)
	{
		UE_LOG(LogAbility, Warning, TEXT("Couldn't spawn actor for %s because no implementation was specified"), *GetClass()->GetName());
		return nullptr;
	}

	AActor* Actor = World->SpawnActor(ImplementationClass, &Location, &Rotator, SpawnParams);
	if (!Actor)
	{
		UE_LOG(LogAbility, Warning, TEXT("Actor for %s has not been spawned, check spawn log for more details"), *GetClass()->GetName());
		return nullptr;
	}

	return Cast<IHAbilityActorInterface>(Actor);
}