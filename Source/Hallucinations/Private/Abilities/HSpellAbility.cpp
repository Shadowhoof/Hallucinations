// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HSpellAbility.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/HCharacter.h"
#include "Constants/HConstants.h"

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
	const FTransform SpawnTransform = GetCastTransform(TargetActor->GetActorLocation());
	CreateInstance(SpawnTransform, TargetActor);
}

void UHSpellAbility::FinishLocationCast(const FVector& TargetLocation)
{
	const FTransform SpawnTransform = GetCastTransform(TargetLocation);
	CreateInstance(SpawnTransform, TargetLocation);
}

void UHSpellAbility::FinishSelfCast()
{
	AHCharacter* Caster = AbilityComponent->GetCaster();
	const FTransform SpawnTransform = GetCastTransform(Caster->GetActorLocation());
	CreateInstance(SpawnTransform, Caster);
}

FTransform UHSpellAbility::GetCastTransform(FVector TargetLocation) const
{
	const AHCharacter* Caster = AbilityComponent->GetCaster();
	const FVector SocketLocation = Caster->GetMesh()->GetSocketLocation(SocketConstants::SpellSocketName);
	TargetLocation.Z = SocketLocation.Z;
	const FRotator Rotation = (TargetLocation - SocketLocation).Rotation();
	return FTransform(Rotation, SocketLocation);
}
