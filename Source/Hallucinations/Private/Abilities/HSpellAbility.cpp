// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HSpellAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/HCharacter.h"

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

float UHSpellAbility::GetCastPoint() const
{
	return CastPoint;
}

float UHSpellAbility::GetCastBackswing() const
{
	return CastBackswing;
}

void UHSpellAbility::FinishActorCast(AActor* TargetActor)
{
	CreateInstance(GetWorld(), TargetActor->GetActorLocation(), TargetActor->GetActorRotation(), TargetActor, nullptr);
}

void UHSpellAbility::FinishLocationCast(FVector TargetLocation)
{
	CreateInstance(GetWorld(), TargetLocation, FRotator(), nullptr, &TargetLocation);
}

void UHSpellAbility::FinishSelfCast()
{
	AActor* Owner = AbilityComponent->GetCaster();
	CreateInstance(GetWorld(), Owner->GetActorLocation(), Owner->GetActorRotation(), Owner, nullptr);
}