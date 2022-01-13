// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HAttackComponent.generated.h"

class UHAbility;
class AHCharacter;
class AHWeapon;

DECLARE_LOG_CATEGORY_EXTERN(LogAttack, Log, All);


UENUM()
enum class EAttackMode : uint8
{
	None,			// attacking nothing
	Actor,			// attacking actor once
	LockedActor,	// attacking actor until lock is released
	Ground			// attacking ground
};


USTRUCT(BlueprintType)
struct FAttackResult
{
	GENERATED_BODY()
	
	/** Actor which was targeted or hit (in case of melee weapons) by an attack */
	TWeakObjectPtr<AActor> Actor;

	/** Location which was targeted during attack */
	FVector Location;

	/**
	 *	Either a location where projectile is spawned (for ranged weapons) or where weapon is
	 *	located during contact frame (for melee weapons).
	 */
	FVector SpawnOrWeaponLocation;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAttackStartedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttackEndedEvent, const FAttackResult&, AttackResult);


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

	// Time in seconds that must pass between two different attacks. Timing begins when attack starts rather than when it lands.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Speed")
	float BaseAttackSpeed = 1.f;
	
	// Base time between attack initiation and attack actually happening before any modifiers are applied, depends on attack animation
	float BaseAttackDelay;
	
	// Actor to attack
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	AActor* TargetActor;

	// Location to attack
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	FVector TargetLocation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	bool bIsAttacking = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	bool bIsAttackOnCooldown = false;

	bool bIsAttackEnabled = true;

	// Is current attack requested by an ability
	bool bIsAbilityAttack;
	
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

	bool bHasAttackedWhileLocked = false;
	bool bIsAttackCancelPending = false;

	void FollowTargetActor();

	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOwnerDeath(AHCharacter* Victim, AActor* Killer);

	bool CanIssueAttackOrder(AActor* Actor) const;
	bool CanIssueAttackOrder(const FVector& Location) const;
	bool CanStartAttack() const;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FAttackStartedEvent OnAttackStarted;

	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FAttackEndedEvent OnAttackEnded;

	DECLARE_EVENT(UHAttackComponent, FAttackInterruptedEvent);
	FAttackInterruptedEvent OnAttackInterrupted;

	DECLARE_EVENT(UHAttackComponent, FAttackCancelledEvent);
	FAttackCancelledEvent OnAttackCancelled;

	/** Locks on target that will be attacked if it's an enemy. Returns whether target is in range of the weapon */
	UFUNCTION(BlueprintCallable)
	bool AttackActor(AActor* Actor);

	/**
	 *	Regular actor attack logic except an ability will handle the logic that executes when attack finishes.
	 *	@return Whether attack was successfully queued (either ready to execute now or moving towards the target)
	 **/
	UFUNCTION(BlueprintCallable)
	bool AttackActorWithAbility(AActor* Actor);
	
	/** Starts attacking target location */
	UFUNCTION(BlueprintCallable)
	void AttackLocation(const FVector& Location);

	/**
	 *	Regular location attack logic except an ability will handle the logic that executes when attack finishes.
	 *	@return Whether attack was successfully queued (either ready to execute now or moving towards the target)
	 **/
	UFUNCTION(BlueprintCallable)
	bool AttackLocationWithAbility(const FVector& Location);

	/** Changes locked actor attack mode to regular actor attack mode */
	void CancelActorLock();
	
	/** Cancels any issued attack orders */
	UFUNCTION(BlueprintCallable)
	void StopAttacking(bool bInterruptAttack = false);

	void EnableAttack();
	void DisableAttack();
	
	AActor* GetTargetActor() const;

	static bool CanBeAttacked(AActor* Attacker, AActor* Victim);

	void HandlePlayerAttack(const FHitResult& MouseoverData, bool bIsRepeated, bool bIsGroundAttack);

	EAttackMode GetCurrentAttackMode() const;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	bool IsAttacking() const;

	AHCharacter* GetCharacter() const;

private:
	
	float CalculateAttackDelay() const;
	float CalculateAttackSpeed() const;

	// constants
	static const float ChilledAttackSpeedMultiplier;
	
};
