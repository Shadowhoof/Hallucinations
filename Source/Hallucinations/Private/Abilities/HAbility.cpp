// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HAbility.h"

#include "HConstants.h"
#include "Abilities/HAbilityComponent.h"
#include "Components/HFollowComponent.h"
#include "Characters/HCharacter.h"
#include "Utils/HEnumTools.h"
#include "Core/HLogCategories.h"
#include "TimerManager.h"


void UHAbility::Use(UHAbilityComponent* Context, AActor* TargetActor)
{
	UE_LOG(LogAbility, Log, TEXT("Using ability %s on actor %s"), *GetClass()->GetName(), TargetActor ? *TargetActor->GetName() : TEXT("NULL"));
	AHCharacter* Caster = Context->GetCaster();
	UHFollowComponent* FollowComponent = Cast<UHFollowComponent>(Caster->GetComponentByClass(UHFollowComponent::StaticClass()));
	FollowComponent->RotateTowardsActor(TargetActor);

	const FTimerDelegate Delegate = FTimerDelegate::CreateUFunction(this, FName("FinishActorCast"), Context, TargetActor);
	DelayCast(Context, Delegate);
}

void UHAbility::Use(UHAbilityComponent* Context, FVector TargetLocation)
{
	UE_LOG(LogAbility, Log, TEXT("Using ability %s at location %s"), *GetClass()->GetName(), *TargetLocation.ToString());
	AHCharacter* Caster = Context->GetCaster();
	UHFollowComponent* FollowComponent = Cast<UHFollowComponent>(Caster->GetComponentByClass(UHFollowComponent::StaticClass()));
	FollowComponent->RotateTowardsLocation(TargetLocation);

	const FTimerDelegate Delegate = FTimerDelegate::CreateUFunction(this, FName("FinishLocationCast"), Context, TargetLocation);
	DelayCast(Context, Delegate);
}

void UHAbility::Use(UHAbilityComponent* Context)
{
	UE_LOG(LogAbility, Log, TEXT("Using ability %s on self"), *GetClass()->GetName());

	const FTimerDelegate Delegate = FTimerDelegate::CreateUFunction(this, FName("FinishSelfCast"), Context);
	DelayCast(Context, Delegate);
}

void UHAbility::DelayCast(UHAbilityComponent* Context, const FTimerDelegate& Delegate)
{
	if (!CastAnimation)
	{
		UE_LOG(LogAbility, Error, TEXT("No animation specified for ability %s"), *GetClass()->GetName());
		return;
	}

	Context->StartCast(GetCastTime(), Delegate);
}

void UHAbility::FinishActorCast(UHAbilityComponent* Context, AActor* TargetActor)
{
	OnCastFinished();
}

void UHAbility::FinishLocationCast(UHAbilityComponent* Context, FVector TargetLocation)
{
	OnCastFinished();
}

void UHAbility::FinishSelfCast(UHAbilityComponent* Context)
{
	OnCastFinished();
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

float UHAbility::GetRemainingCooldown(const UObject* WorldContextObject) const
{
	if (!GEngine)
	{
		UE_LOG(LogAbility, Error, TEXT("Pointer to GEngine is null"));
		return 0.f;
	}
	
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	return World->GetTimerManager().GetTimerRemaining(CooldownTimerHandle);
}

float UHAbility::GetRemainingCooldownPercentage(const UObject* WorldContextObject) const
{
	if (Cooldown <= 0.f)
	{
		return 0.f;
	}

	return GetRemainingCooldown(WorldContextObject) / Cooldown;
}

bool UHAbility::TryUse(UHAbilityComponent* Context)
{
	FTimerManager& TimerManager = Context->GetWorld()->GetTimerManager();
	if (TimerManager.IsTimerActive(CooldownTimerHandle))
	{
		// ability is still on cooldown
		UE_LOG(LogAbility, Log, TEXT("Ability is on cooldown, %.2f remaining"), TimerManager.GetTimerRemaining(CooldownTimerHandle));
		return false;
	}

	bool bWasUsed = false;
	if (TEST_BIT(TargetType, EAbilityTarget::Actor))
	{
		AActor* TargetActor = Context->GetTargetActor();
		if (TargetActor)
		{
			Use(Context, TargetActor);
			bWasUsed = true;
		}
	}

	if (!bWasUsed && TEST_BIT(TargetType, EAbilityTarget::Point))
	{
		const FVector TargetLocation = Context->GetTargetLocation();
		if (TargetLocation != FHConstants::NullVector)
		{
			Use(Context, TargetLocation);
			bWasUsed = true;
		}
	}

	if (!bWasUsed && TEST_BIT(TargetType, EAbilityTarget::Self))
	{
		Use(Context);
		bWasUsed = true;
	}

	if (bWasUsed)
	{
		OnCastStarted(Context->GetCaster());
	}

	//UE_LOG(LogAbility, VeryVerbose, TEXT("Tried to use ability %s, result: %s"), *GetName())
	UE_LOG(LogAbility, Log, TEXT("Actor %s tried to use ability %s, result: %s"), *Context->GetCaster()->GetName(),
		*GetClass()->GetName(), bWasUsed ? TEXT("true") : TEXT("false"));
	return bWasUsed;
}

FText UHAbility::GetSkillName() const
{
	return Name.IsEmpty() ? FText::FromString(GetClass()->GetName()) : Name;
}

void UHAbility::OnCastStarted(AHCharacter* Caster)
{
	if (CastAnimation)
	{
		Caster->PlayAnimMontage(CastAnimation);
	}
}

void UHAbility::OnCastFinished()
{
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, Cooldown, false);
}

float UHAbility::GetCastTime() const
{
	if (!CastAnimation)
	{
		UE_LOG(LogAbility, Error, TEXT("No animation set for ability %s"), *GetClass()->GetName());
		return 0.f;
	}
	
	return CastAnimation->GetSectionLength(0);
}