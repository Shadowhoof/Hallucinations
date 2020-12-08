// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HCharacter.generated.h"

class UHFollowComponent;
class AHWeapon;
class UHHealthComponent;
class UHAttackComponent;

UCLASS()
class HALLUCINATIONS_API AHCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHCharacter();

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHAttackComponent* AttackComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHFollowComponent* FollowComponent;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnDeath(AActor* Victim, AActor* Killer);

	UFUNCTION()
	void OnAttackStart();

	UFUNCTION()
	void OnAttackEnd();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool IsDead() const;

	virtual FVector GetTargetLocation(AActor* RequestedBy) const override;
};
