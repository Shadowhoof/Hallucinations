// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HAttackComponent.generated.h"

class AHCharacter;
class AHWeapon;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackEnd);

DECLARE_LOG_CATEGORY_EXTERN(LogAttack, Log, All);

UENUM()
enum class EAttackMode : uint8
{
	None,			// attacking nothing
	Actor,			// attacking actor once
	LockedActor,	// attacking actor until lock is released
	Ground			// attacking ground
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HALLUCINATIONS_API UHAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHAttackComponent();

protected:

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AHWeapon> WeaponClass;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	AHWeapon* Weapon;

	// Time between attack initiation and attack actually happening, depends on attack animation
	float AttackDelay;

	// Actor to attack
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	AActor* TargetActor;

	// Location to attack
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	FVector TargetLocation;

	bool bIsAttacking;
	bool bIsAttackOnCooldown;
	
	// Begins attack animation
	void StartAttack();

	void PerformAttack();

	// When attack cooldown expires
	void OnAttackCooldownOver();
	
	// Timer handle for attack animation
	FTimerHandle AttackTimerHandle;

	// Timer handle for attack cooldown
	FTimerHandle AttackCooldownTimerHandle;

	EAttackMode AttackMode;

	bool bHasAttackedWhileLocked;

	void FollowTargetActor();

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FOnAttackStart AttackStartedEvent;

	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FOnAttackEnd AttackEndedEvent;

	/** Locks on target that will be attacked if it's an enemy. Returns whether target is in range of the weapon */
	UFUNCTION(BlueprintCallable)
	void AttackActor(AActor* Actor);

	/** Starts attacking target location */
	UFUNCTION(BlueprintCallable)
	void AttackLocation(const FVector& Location);

	/** Changes locked actor attack mode to regular actor attack mode */
	void CancelActorLock();
	
	/** Cancels any issued attack orders */
	UFUNCTION(BlueprintCallable)
	void StopAttacking();

	AActor* GetTargetActor() const;

	static bool CanBeAttacked(AActor* Attacker, AActor* Victim);

	void HandlePlayerAttack(const FHitResult& MouseoverData, bool bIsRepeated, bool bIsGroundAttack);

	EAttackMode GetCurrentAttackMode() const;
};
