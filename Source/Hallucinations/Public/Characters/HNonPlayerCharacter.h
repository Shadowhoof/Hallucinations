// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HCharacter.h"
#include "Core/Subsystems/Save/HStatefulActorInterface.h"

#include "HNonPlayerCharacter.generated.h"


/**
 * Represents a non-player character (NPC). Can be either friendly or hostile.
 * Attack values are directly settable through this class in order to make modifying
 * instances of this object easier.
 */
UCLASS()
class HALLUCINATIONS_API AHNonPlayerCharacter : public AHCharacter, public IHStatefulActorInterface
{
	GENERATED_BODY()

public:

	AHNonPlayerCharacter();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void EnterCombat();

	UFUNCTION(BlueprintCallable, Category = "Combat")
	void LeaveCombat();

	virtual bool ShouldBeSaved() const override;
	virtual FPersistentActorState GetPersistentState() const override;
	virtual void RestorePersistentState(const FPersistentActorState& State) override;

	virtual AActor* GetTargetActor() const override;
	
protected:

	virtual void BeginPlay() override;

	virtual void OnDeath(AActor* Victim, AActor* Killer) override;
	
	UPROPERTY(Transient)
	bool bInCombat = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CombatMovementSpeed = 450.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float NonCombatMovementSpeed = 250.f;
	
};
