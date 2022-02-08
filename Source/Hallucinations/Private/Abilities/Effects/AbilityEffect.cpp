// Created by Dmitrii Korovin


#include "Abilities/Effects/AbilityEffect.h"

#include "Abilities/HAbilityStatics.h"
#include "Constants/HConstants.h"
#include "Core/HLogCategories.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogAbilityEffect)

UAbilityEffect::UAbilityEffect()
{
}

void UAbilityEffect::ApplyToActor(AActor* InInstigatorActor, AController* InInstigatorController, AActor* HitActor,
                                  const FHitResult* HitResult)
{
	UE_LOG(LogAbilityEffect, Verbose, TEXT("Applying effect %s to actor %s"), *GetName(), *HitActor->GetName());
}

void UAbilityEffect::ApplyToLocation(AActor* InInstigatorActor, AController* InInstigatorController,
                                     const FVector& HitLocation, const FHitResult* HitResult)
{
	if (Radius <= 0.f)
	{
		return;
	}

	UE_LOG(LogAbilityEffect, Verbose, TEXT("Applying effect %s to location %s"), *GetName(), *HitLocation.ToString());
	
	TSet<AActor*> ActorsInRadius;
	UHAbilityStatics::GetActorsInRadius(ActorsInRadius, GetWorld(), HitLocation, Radius);
	for (AActor* Actor : ActorsInRadius)
	{
		ApplyToActor(InInstigatorActor, InInstigatorController, Actor, nullptr);
	}
}

void UAbilityEffect::Apply(AActor* InInstigatorActor, AController* InInstigatorController,
                           const FAbilityTargetParameters& TargetParams, const FHitResult* HitResult)
{
	if (Radius > 0.f)
	{
		FVector Location = TargetParams.Location;
		if (Location == HallucinationsConstants::InvalidVector)
		{
			if (const AActor* HitActor = TargetParams.Actor.Get())
			{
				Location = HitActor->GetActorLocation();
			}
		}

		if (Location != HallucinationsConstants::InvalidVector)
		{
			ApplyToLocation(InInstigatorActor, InInstigatorController, Location, HitResult);
		}
	}
	else
	{
		if (AActor* HitActor = TargetParams.Actor.Get())
		{
			ApplyToActor(InInstigatorActor, InInstigatorController, HitActor, HitResult);
		}
	}
}
