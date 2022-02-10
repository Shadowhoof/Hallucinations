// Created by Dmitrii Korovin


#include "Abilities/Effects/AbilityEffect.h"

#include "Abilities/HAbilityStatics.h"
#include "Constants/HConstants.h"
#include "Core/HLogCategories.h"
#include "Kismet/GameplayStatics.h"
#include "Constants/HTeamConstants.h"

DEFINE_LOG_CATEGORY(LogAbilityEffect)

UAbilityEffect::UAbilityEffect() : Radius(0.f), AffectedTargets(static_cast<uint8>(EThreatStatus::Enemy))
{
}

void UAbilityEffect::ApplyToActor(AActor* InInstigatorActor, AController* InInstigatorController, AActor* HitActor,
                                  const FHitResult& HitResult)
{
	UE_LOG(LogAbilityEffect, Verbose, TEXT("Applying effect %s to actor %s"), *GetName(), *HitActor->GetName());
}

void UAbilityEffect::ApplyToLocation(AActor* InInstigatorActor, AController* InInstigatorController,
                                     const FVector& HitLocation, const FHitResult& HitResult)
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
		ApplyToActor(InInstigatorActor, InInstigatorController, Actor, FHitResult());
	}
}

void UAbilityEffect::Apply(const FAbilityEffectParameters& Params)
{
	if (Radius > 0.f)
	{
		FVector Location = Params.TargetLocation;
		if (Location == HallucinationsConstants::InvalidVector)
		{
			if (const AActor* HitActor = Params.TargetActor)
			{
				Location = HitActor->GetActorLocation();
			}
		}

		if (Location != HallucinationsConstants::InvalidVector)
		{
			ApplyToLocation(Params.InstigatorActor, Params.InstigatorController, Location, Params.HitResult);
		}
	}
	else
	{
		if (AActor* HitActor = Params.TargetActor)
		{
			ApplyToActor(Params.InstigatorActor, Params.InstigatorController, HitActor, Params.HitResult);
		}
	}
}
