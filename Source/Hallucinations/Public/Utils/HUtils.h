// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "HUtils.generated.h"

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
};
