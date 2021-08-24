// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HSpellAbility.h"
#include "Core/HLogCategories.h"

UHSpellAbility::UHSpellAbility()
{
	Type = EAbilityType::Spell;
}

UAnimMontage* UHSpellAbility::GetCastAnimation() const
{
	return CastAnimation;
}

void UHSpellAbility::OnCastFinished(const FAbilityTargetParameters& TargetParams)
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

void UHSpellAbility::FinishActorCast(AActor* TargetActor)
{
}

void UHSpellAbility::FinishLocationCast(FVector TargetLocation)
{
}

void UHSpellAbility::FinishSelfCast()
{
}

IHAbilityActorInterface* UHSpellAbility::CreateActor(UWorld* World, FVector& Location, FRotator& Rotator,
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