// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HFollowComponent.generated.h"


class IHInteractable;
class AHCharacter;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HALLUCINATIONS_API UHFollowComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UHFollowComponent();

protected:
	
	virtual void BeginPlay() override;

	bool bIsMovementLocked;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	AActor* RotationActor;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	FVector RotationLocation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	AActor* FollowedActor;

	UFUNCTION()
	void OnOwnerDeath(AHCharacter* Victim, AActor* Killer);

	bool CanIssueMoveOrder() const;

	bool CanRotate() const;
	
public:

	UFUNCTION(BlueprintCallable)
	void MoveToActor(AActor* const Actor);

	UFUNCTION(BlueprintCallable)
	void MoveToLocation(const FVector& Location);

	void StopMovement();

	void LockMovement();
	void UnlockMovement();

	UFUNCTION(BlueprintCallable)
	void RotateTowardsActor(AActor* const Actor);

	UFUNCTION(BlueprintCallable)
	void RotateTowardsLocation(FVector Location);

	UFUNCTION(BlueprintCallable)
	void StopRotation();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	static bool CanBeFollowed(AActor* Target);

	AHCharacter* GetCharacter() const;

	void Interrupt();

};
