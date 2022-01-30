// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapons/HWeapon.h"
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


UENUM()
enum class EStopAttackReason
{
	/** Attack was queued but cancelled before it happened */
	Cancel,
	/** Attack was successfully performed and the attack mode was not EAttackMode::LockedActor */
	Success,
	/** Attack was hard interrupted (i.e. by a stun or owner death) */
	Interrupt
};


UENUM(BlueprintType)
enum class EAttackRequestResult : uint8
{
	/** Attack request was denied, no attack will happen */
	Denied,
	/** Attack can't happen right now but it was put into queue and will happen when possible */
	Queued,
	/** Attack has already started */
	Started,
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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttackPointReachedEvent, const FAttackResult&, AttackResult);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HALLUCINATIONS_API UHAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UHAttackComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FAttackStartedEvent OnAttackStarted;

	UPROPERTY(BlueprintAssignable, Category = "Attack")
	FAttackPointReachedEvent OnAttackPointReached;

	DECLARE_EVENT(UHAttackComponent, FAttackInterruptedEvent);
	FAttackInterruptedEvent OnAttackInterrupted;

	DECLARE_EVENT(UHAttackComponent, FAttackCancelledEvent);
	FAttackCancelledEvent OnAttackCancelled;

	DECLARE_EVENT(UHAttackComponent, FAttackBackswingFinished);
	FAttackBackswingFinished OnAttackBackswingFinished;
	
	/** Locks on target that will be attacked if it's an enemy. Returns whether target is in range of the weapon */
	UFUNCTION(BlueprintCallable)
	EAttackRequestResult AttackActor(AActor* Actor, bool bAttackOnce);

	/**
	 *	Regular actor attack logic except an ability will handle the logic that executes when attack finishes.
	 *	@return Whether attack was successfully queued (either ready to execute now or moving towards the target)
	 **/
	UFUNCTION(BlueprintCallable)
	EAttackRequestResult AttackActorWithAbility(AActor* Actor);
	
	/** Starts attacking target location */
	UFUNCTION(BlueprintCallable)
	EAttackRequestResult AttackLocation(const FVector& Location);

	/**
	 *	Regular location attack logic except an ability will handle the logic that executes when attack finishes.
	 *	@return Whether attack was successfully queued (either ready to execute now or moving towards the target)
	 **/
	UFUNCTION(BlueprintCallable)
	EAttackRequestResult AttackLocationWithAbility(const FVector& Location);

	/** Changes locked actor attack mode to regular actor attack mode */
	void CancelActorLock();
	
	/** Cancels any issued attack orders */
	void StopAttacking(const EStopAttackReason StopReason);

	void EnableAttack();
	void DisableAttack();
	
	AActor* GetTargetActor() const;

	static bool CanBeAttacked(AActor* Attacker, AActor* Victim);

	void HandlePlayerAttack(const FHitResult& MouseoverData, bool bIsRepeated, bool bIsGroundAttack);

	EAttackMode GetCurrentAttackMode() const;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

	bool IsAttacking() const;

	AHCharacter* GetCharacter() const;

protected:

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AHWeapon> WeaponClass;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	AHWeapon* Weapon;

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

	/** Is current attack requested by an ability */
	bool bIsAbilityAttack;
	
	/** Begins attack animation */
	bool StartAttack();

	void PerformAttack();

	void FinishAttackBackswing();
	
	/** When attack cooldown expires */
	void OnAttackCooldownOver();
	
	EAttackMode AttackMode;

	bool bHasAttackedWhileLocked = false;
	bool bIsAttackCancelPending = false;

	void FollowTargetActor();

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOwnerDeath(AHCharacter* Victim, AActor* Killer);

	bool CanIssueAttackOrder(AActor* Actor) const;
	bool CanIssueAttackOrder(const FVector& Location) const;
	bool CanStartAttack() const;
	
private:
	
	float CalculateAttackPoint() const;
	float CalculateAttackBackswing() const;
	float CalculateAttackSpeed() const;

	void InterruptAttack();
	
	FWeaponAttackParameters WeaponParams;

	FTimerHandle AttackPointHandle;
	FTimerHandle AttackBackswingHandle;
	FTimerHandle AttackCooldownHandle;
	
	// constants
	static const float ChilledAttackSpeedMultiplier;
	
};
