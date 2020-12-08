// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HFollowComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HALLUCINATIONS_API UHFollowComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHFollowComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool bIsMovementLocked;

	UPROPERTY()
	AActor* RotationActor;
	
	UPROPERTY()
	FVector RotationLocation;

	void ResetRotationFields();

public:
	
	void MoveTo(AActor* const Actor);
	void MoveTo(const FVector& Location);

	void StopMovement();

	void LockMovement();
	void UnlockMovement();

	void RotateTowards(AActor* const Actor);
	void RotateTowards(FVector Location);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	static bool CanBeFollowed(AActor* Target);
};
