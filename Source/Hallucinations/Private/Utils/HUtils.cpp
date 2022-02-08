// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/HUtils.h"

#include "Constants/HConstants.h"
#include "NavigationSystem.h"
#include "Characters/HCharacter.h"
#include "Components/HHealthComponent.h"

class AHCharacter;

bool UHUtils::CanBeTargeted(AActor* TargetActor)
{
	if (!TargetActor)
	{
		return false;
	}

	UHHealthComponent* HealthComponent = Cast<UHHealthComponent>(TargetActor->GetComponentByClass(UHHealthComponent::StaticClass()));
	return HealthComponent && !HealthComponent->IsDead();
}

bool UHUtils::CanBeAttacked(AActor* Attacker, AActor* Victim)
{
	return CanBeTargeted(Victim) && AreEnemies(Attacker, Victim);
}

bool UHUtils::AreEnemies(AActor* First, AActor* Second)
{
	return UHHealthComponent::AreEnemies(First, Second);
}

bool UHUtils::IsVectorValid(const FVector& Vector)
{
	return Vector != FHConstants::NullVector;
}

void UHUtils::GetRandomPointInNavigableRadius(UObject* WorldContextObject, const FVector& Origin, float Radius, FVector& OutLocation)
{
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(WorldContextObject->GetWorld());
	FNavLocation RandomNavLocation;
	NavSystem->GetRandomPointInNavigableRadius(Origin, Radius, RandomNavLocation);
	OutLocation = RandomNavLocation.Location;
}

bool UHUtils::DoesSatisfyThreatStatus(const AActor* First, const AActor* Second, const EThreatStatus ThreatStatus)
{
	return static_cast<uint8>(UHHealthComponent::GetThreatStatus(First, Second) & ThreatStatus) > 0;
}

