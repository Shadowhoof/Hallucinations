// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HAbility.h"

#include "Abilities/HAbilityComponent.h"
#include "Components/HFollowComponent.h"
#include "Characters/HCharacter.h"
#include "Utils/HEnumTools.h"
#include "Core/HLogCategories.h"
#include "TimerManager.h"
#include "Abilities/Instances/InstantAbilityInstance.h"
#include "Constants/HConstants.h"
#include "Constants/HTeamConstants.h"
#include "Utils/HUtils.h"


UHAbility::UHAbility() : InstanceClass(UInstantAbilityInstance::StaticClass())
{
}

float UHAbility::GetRemainingCooldown() const
{	
	return AbilityComponent->GetWorld()->GetTimerManager().GetTimerRemaining(CooldownTimerHandle);
}

float UHAbility::GetRemainingCooldownPercentage() const
{
	return (Cooldown > 0.f) ? (GetRemainingCooldown() / Cooldown) : 0.f;
}

bool UHAbility::IsOnCooldown() const
{
	return GetRemainingCooldown() > 0.f;
}

void UHAbility::RestoreCooldownPercentage(float CooldownPercentage)
{
	const float CooldownInSeconds = Cooldown * FMath::Clamp(CooldownPercentage, 0.f, 1.f);
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, CooldownInSeconds, false);
}

bool UHAbility::IsOffensive() const
{
	return static_cast<bool>(static_cast<EThreatStatus>(AffectedTargets) & EThreatStatus::Enemy);
}

const TArray<UAbilityEffect*>& UHAbility::GetEffects() const
{
	return Effects;
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

float UHAbility::GetManaCost() const
{
	return ManaCost;
}

void UHAbility::SetAbilityComponent(UHAbilityComponent* Component)
{
	ensure(!AbilityComponent && Component);
	AbilityComponent = Component;
}

void UHAbility::CreateInstance(const FTransform& SpawnTransform, AActor* TargetActor)
{
	CreateInstanceImpl(SpawnTransform, TargetActor, HallucinationsConstants::InvalidVector);
}

void UHAbility::CreateInstance(const FTransform& SpawnTransform, const FVector& TargetLocation)
{
	CreateInstanceImpl(SpawnTransform, nullptr, TargetLocation);
}

void UHAbility::CreateInstanceImpl(const FTransform& SpawnTransform, AActor* TargetActor, const FVector& TargetLocation)
{
	UAbilityInstance* Instance = NewObject<UAbilityInstance>(this, InstanceClass);

	AHCharacter* Caster = AbilityComponent->GetCaster();
	ensureMsgf(Caster, TEXT("Ability %s has no caster"), *GetName());
	const FAbilityEffectParameters EffectParams{Caster, Caster->GetInstigatorController(), TargetActor, TargetLocation, FHitResult(), SpawnTransform}; 
	Instance->Initialize(this, EffectParams);
}
