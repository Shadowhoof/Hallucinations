// Fill out your copyright notice in the Description page of Project Settings.


#include "StatusEffects/HStatusEffect.h"
#include "Core/HLogCategories.h"

UHStatusEffectComponent* UHStatusEffect::GetComponent() const
{
	return Cast<UHStatusEffectComponent>(AffectedActor->GetComponentByClass(UHStatusEffectComponent::StaticClass()));
}

void UHStatusEffect::Begin(AActor* Affected, AActor* Source, float Duration)
{
	AffectedActor = Affected;
	SourceActor = Source;
	InitialDuration = Duration;
	RemainingDuration = Duration;

	bIsActive = true;

	UE_LOG(LogStatusEffect, Log, TEXT("Status effect %s applied"), *GetClass()->GetName())
}

void UHStatusEffect::End()
{
	bIsActive = false;
	RemainingDuration = 0.f;
	UE_LOG(LogStatusEffect, Log, TEXT("Status effect %s removed"), *GetClass()->GetName())
}

void UHStatusEffect::Tick(float DeltaTime)
{
	if (!bIsActive)
	{
		return;
	}
	
	RemainingDuration -= DeltaTime;
	if (RemainingDuration < 0.f)
	{
		End();
	}
}

float UHStatusEffect::GetRemainingDuration() const
{
	return FMath::Max(RemainingDuration, 0.f);
}

float UHStatusEffect::GetRemainingPercentage() const
{
	if (InitialDuration == 0)
	{
		return 0.f;
	}

	return FMath::Clamp(RemainingDuration / InitialDuration, 0.f, 1.f);
}

uint8 UHStatusEffect::GetActiveConditions() const
{
	return 0;
}

bool UHStatusEffect::IsActive() const
{
	return bIsActive;
}
