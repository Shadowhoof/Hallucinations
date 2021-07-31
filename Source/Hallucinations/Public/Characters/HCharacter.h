// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StatusEffects/HStatusEffectComponent.h"
#include "HCharacter.generated.h"

class UBoxComponent;
class UHAbilityComponent;
class UHStatusEffectComponent;
class AHFireballActor;
class UHFollowComponent;
class AHWeapon;
class UHHealthComponent;
class UHAttackComponent;
class UHAttributeComponent;
class AHCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FCharacterDeathEvent, AHCharacter*, Victim, AActor*, Killer);

UCLASS()
class HALLUCINATIONS_API AHCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHCharacter();

protected:

	/** Used for mouse targeting raycast */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BoxComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHAttackComponent* AttackComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHFollowComponent* FollowComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHStatusEffectComponent* StatusEffectComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHAbilityComponent* AbilityComponent;

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnDeath(AActor* Victim, AActor* Killer);

	UFUNCTION()
	void OnAttackStart();

	UFUNCTION()
	void OnAttackEnd();

	UFUNCTION()
	void OnConditionApplied(EStatusCondition Condition);

	UFUNCTION()
	void OnConditionRemoved(EStatusCondition Condition);
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	float MinDamage = 0.f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float MaxDamage = 0.f;

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackSpeed = 1.f;

public:
	
	UFUNCTION(BlueprintCallable)
	bool IsDead() const;

	virtual FVector GetTargetLocation(AActor* RequestedBy) const override;

	UPROPERTY(BlueprintAssignable, Category = "Death")
	FCharacterDeathEvent DeathEvent;

	virtual float GetCurrentDamage() const;

	virtual float GetAttackSpeed() const;

	virtual AActor* GetTargetActor() const;

	virtual FVector GetTargetLocation() const;

	// component getters
	UHHealthComponent* GetHealthComponent() const;
	UHAttackComponent* GetAttackComponent() const;
	UHFollowComponent* GetFollowComponent() const;
	UHStatusEffectComponent* GetStatusEffectComponent() const;
	UHAbilityComponent* GetAbilityComponent() const;
	
	void UseAbility(uint8 Index);

	UFUNCTION(BlueprintCallable)
	bool IsStunned() const;

	/** Determines whether character is currently busy and is unable to be interrupted by any other commands
	 * until its current task is finished (i.e. it's in the middle of an attack or in the middle of a spell cast
	 * or it's stunned and is unable to act altogether) */
	bool IsBusy() const;
	
};
