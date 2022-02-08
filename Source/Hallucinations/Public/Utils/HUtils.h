// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HUtils.generated.h"

enum class EThreatStatus : uint8;

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API UHUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "Actor")
	static bool CanBeTargeted(AActor* TargetActor);

	UFUNCTION(BlueprintPure, Category = "Actor")
	static bool CanBeAttacked(AActor* Attacker, AActor* Victim);

	UFUNCTION(BlueprintPure, Category = "Actor")
	static bool AreEnemies(AActor* First, AActor* Second);

	static bool IsVectorValid(const FVector& Vector);

	static void GetRandomPointInNavigableRadius(UObject* WorldContextObject, const FVector& Origin, float Radius, FVector& OutLocation);

	/** Determines whether threat status between two actors is included in provided threat status */
	static bool DoesSatisfyThreatStatus(const AActor* First, const AActor* Second, const EThreatStatus ThreatStatus);
	
	/** Returns a pointer to a const random element from an array. */
	template<typename T>
	static const T* GetRandomArrayElement(const TArray<T>& Array);
};

template <typename T>
const T* UHUtils::GetRandomArrayElement(const TArray<T>& Array)
{
	const int32 ArraySize = Array.Num();
	if (ArraySize == 0)
	{
		return nullptr;
	}

	return &Array[FMath::RandRange(0, ArraySize - 1)];
}
