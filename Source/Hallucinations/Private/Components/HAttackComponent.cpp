// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HAttackComponent.h"
#include "Components/HHealthComponent.h"
#include "Components/HFollowComponent.h"
#include "Weapons/HWeapon.h"
#include "Characters/HCharacter.h"
#include "Utils/HUtils.h"
#include "HConstants.h"
#include "Engine/World.h"
#include "Utils/HLogUtils.h"

DEFINE_LOG_CATEGORY(LogAttack);

const float UHAttackComponent::ChilledAttackSpeedMultiplier = 2.0;

// Sets default values for this component's properties
UHAttackComponent::UHAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	BaseAttackDelay = 1.f;

	TargetActor = nullptr;
	TargetLocation = FHConstants::NullVector;
}

void UHAttackComponent::FollowTargetActor()
{
	const bool bIsInRange = Weapon->IsInRange(GetOwner(), TargetActor);
	UHFollowComponent* FollowComponent = GetCharacter()->GetFollowComponent();
	if (bIsInRange) 
	{
		FollowComponent->RotateTowardsActor(TargetActor);
	}
	else 
	{
		FollowComponent->MoveToActor(TargetActor);
	}
}

// Called when the game starts
void UHAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	AHCharacter* CharacterOwner = GetCharacter();
	CharacterOwner->DeathEvent.AddDynamic(this, &UHAttackComponent::OnOwnerDeath);
	
	if (WeaponClass) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = CharacterOwner;
		SpawnParams.Instigator = CharacterOwner;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Weapon = GetWorld()->SpawnActor<AHWeapon>(WeaponClass, SpawnParams);

		if (Weapon) {
			Weapon->SetOwner(CharacterOwner);

			const FName SocketName = Weapon->GetAttachmentSocketName();
			if (CharacterOwner && !SocketName.IsNone()) {
				Weapon->AttachToComponent(CharacterOwner->GetMesh(),
					FAttachmentTransformRules::SnapToTargetNotIncludingScale,
					Weapon->GetAttachmentSocketName());
			}

			UAnimMontage* AttackAnimation = Weapon->GetAttackAnimation();
			if (AttackAnimation) {
				BaseAttackDelay = AttackAnimation->GetSectionLength(0);
			}
		} else {
			UE_LOG(LogAttack, Warning, TEXT("Actor %s with attack component has no weapon"), *GetOwner()->GetName());
		}
	}
	else
	{
		UE_LOG(LogAttack, Log, TEXT("Actor %s with attack component has no weapon class specified"), *GetOwner()->GetName());
	}
}

void UHAttackComponent::OnOwnerDeath(AHCharacter* Victim, AActor* Killer)
{
	StopAttacking();
}

bool UHAttackComponent::CanIssueAttackOrder(AActor* Actor) const
{
	if (!Weapon || bIsAttacking || GetCharacter()->IsBusy())
	{
		return false;
	}

	if (!UHUtils::AreEnemies(GetOwner(), Actor))
	{
		return false;
	}

	if (Actor == TargetActor && AttackMode == EAttackMode::LockedActor)
	{
		return false;
	}
	
	return true;
}

bool UHAttackComponent::CanIssueAttackOrder(const FVector& Location) const
{
	return Weapon && !bIsAttacking && !GetCharacter()->IsBusy();
}

bool UHAttackComponent::CanStartAttack() const
{
	return Weapon && !bIsAttacking && !bIsAttackOnCooldown && !GetCharacter()->IsBusy();
}

void UHAttackComponent::AttackActor(AActor* Actor)
{
	if (!CanIssueAttackOrder(Actor))
	{
		return;
	}

	UE_LOG(LogAttack, Log, TEXT("%s is attacking actor %s"), *GetOwner()->GetName(), *Actor->GetName());
	TargetActor = Actor;
	TargetLocation = FHConstants::NullVector;
	AttackMode = EAttackMode::LockedActor;

	const bool bIsInRange = Weapon->IsInRange(GetOwner(), TargetActor);
	FollowTargetActor();

	if (bIsInRange)
	{
		StartAttack();
	}
}

bool UHAttackComponent::AttackActorWithAbility(AActor* Actor)
{
	if (!CanIssueAttackOrder(Actor))
	{
		return false;
	}
	
	bIsAbilityAttack = true;
	AttackActor(Actor);
	return true;
}

void UHAttackComponent::AttackLocation(const FVector& Location)
{
	if (!CanIssueAttackOrder(Location))
	{
		return;
	}

	UE_LOG(LogAttack, Verbose, TEXT("%s is attacking location: %s"), *GetOwner()->GetName(), *Location.ToString());
	TargetLocation = Location;
	TargetActor = nullptr;
	AttackMode = EAttackMode::Ground;

	GetCharacter()->GetFollowComponent()->RotateTowardsLocation(Location);

	StartAttack();
}

bool UHAttackComponent::AttackLocationWithAbility(const FVector& Location)
{
	if (!CanIssueAttackOrder(Location))
	{
		return false;
	}

	bIsAbilityAttack = true;
	AttackLocation(Location);
	return true;
}

void UHAttackComponent::StopAttacking(bool bInterruptAttack)
{
	if (AttackMode == EAttackMode::None)
	{
		// we're currently not attacking, return
		return;
	}

	if (bIsAttacking)
	{
		if (!bInterruptAttack)
		{
			// we're in the middle of an attack and this is not an interrupt, wait for it to finish, then cancel
			bIsAttackCancelPending = true;
			return;
		}
		
		GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);
		GetCharacter()->StopAnimMontage(Weapon->GetAttackAnimation());
		bIsAttacking = false;
	}
	
	UE_LOG(LogAttack, Verbose, TEXT("%s stopped attacking"), *GetOwner()->GetName())
	TargetLocation = FHConstants::NullVector;
	TargetActor = nullptr;
	AttackMode = EAttackMode::None;
	bIsAbilityAttack = false;
	bHasAttackedWhileLocked = false;
	bIsAttackCancelPending = false;

	GetCharacter()->GetFollowComponent()->StopMovement();
}

void UHAttackComponent::EnableAttack()
{
	bIsAttackEnabled = true;
}

void UHAttackComponent::DisableAttack()
{
	bIsAttackEnabled = false;
	StopAttacking(true);
}

void UHAttackComponent::CancelActorLock()
{
	if (AttackMode == EAttackMode::LockedActor)
	{
		AttackMode = EAttackMode::Actor;
		if (bHasAttackedWhileLocked)
		{
			StopAttacking();
		}
	}
}

AActor* UHAttackComponent::GetTargetActor() const
{
	return TargetActor;
}

bool UHAttackComponent::CanBeAttacked(AActor* Attacker, AActor* Victim)
{
	return UHHealthComponent::AreEnemies(Attacker, Victim);
}

void UHAttackComponent::HandlePlayerAttack(const FHitResult& MouseoverData, bool bIsRepeated, bool bIsGroundAttack)
{
	if (!MouseoverData.bBlockingHit || GetCharacter()->IsBusy())
	{
		return;
	}

	AActor* MouseoverActor = MouseoverData.GetActor();
	bool bIsAttackable = CanBeAttacked(GetOwner(), MouseoverActor);
	if (bIsGroundAttack)
	{
		FVector Location = bIsAttackable ? MouseoverActor->GetActorLocation() : MouseoverData.ImpactPoint;
		AttackLocation(Location);
	}
	else if (!bIsRepeated && bIsAttackable)
	{
		AttackActor(MouseoverActor);
	}
}

EAttackMode UHAttackComponent::GetCurrentAttackMode() const
{
	return AttackMode;
}

void UHAttackComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (Weapon)
	{
		GetWorld()->DestroyActor(Weapon);
	}
}

bool UHAttackComponent::IsAttacking() const
{
	return bIsAttacking;
}

AHCharacter* UHAttackComponent::GetCharacter() const
{
	return Cast<AHCharacter>(GetOwner());
}

float UHAttackComponent::CalculateAttackDelay() const
{
	bool IsChilled = GetCharacter()->GetStatusEffectComponent()->IsConditionActive(EStatusCondition::Chilled);
	return IsChilled ? BaseAttackDelay * ChilledAttackSpeedMultiplier : BaseAttackDelay;
}

float UHAttackComponent::CalculateAttackSpeed() const
{
	bool IsChilled = GetCharacter()->GetStatusEffectComponent()->IsConditionActive(EStatusCondition::Chilled);
	return IsChilled ? BaseAttackSpeed * ChilledAttackSpeedMultiplier : BaseAttackSpeed;
}

void UHAttackComponent::StartAttack()
{
	if (!CanStartAttack())
	{
		return;
	}

	float AttackDelay = CalculateAttackDelay();
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &UHAttackComponent::PerformAttack, AttackDelay);
	bIsAttacking = true;

	float AttackSpeed = CalculateAttackSpeed();
	GetWorld()->GetTimerManager().SetTimer(AttackCooldownTimerHandle, this, &UHAttackComponent::OnAttackCooldownOver, AttackSpeed);
	bIsAttackOnCooldown = true;
	
	// Disable movement until contact frame
	GetCharacter()->GetFollowComponent()->LockMovement();

	// Start attack animation
	UAnimMontage* AttackAnimation = Weapon->GetAttackAnimation();
	if (AttackAnimation)
	{
		float AnimationPlayRate = BaseAttackSpeed / AttackSpeed;
		GetCharacter()->PlayAnimMontage(AttackAnimation, AnimationPlayRate);
	}
	
	AttackStartedEvent.Broadcast();
}

void UHAttackComponent::PerformAttack()
{
	ensure(TargetLocation != FHConstants::NullVector || TargetActor);

	GetCharacter()->GetFollowComponent()->UnlockMovement();

	FAttackResult ResultParams;
	bIsAttacking = false;
	switch (AttackMode)
	{
	case EAttackMode::Actor:
		Weapon->AttackActor(TargetActor, bIsAbilityAttack, ResultParams);
		StopAttacking();
		break;
	case EAttackMode::LockedActor:
		Weapon->AttackActor(TargetActor, bIsAbilityAttack, ResultParams);
		bHasAttackedWhileLocked = true;
		break;
	case EAttackMode::Ground:
		Weapon->AttackLocation(TargetLocation, bIsAbilityAttack, ResultParams);
		TargetLocation = FHConstants::NullVector;
		break;
	case EAttackMode::None:
		break;
	}

	if (bIsAttackCancelPending)
	{
		StopAttacking();
	}

	AttackEndedEvent.Broadcast(ResultParams);
}

void UHAttackComponent::OnAttackCooldownOver()
{
	bIsAttackOnCooldown = false;
}

// Called every frame
void UHAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsAttacking)
	{
		return;
	}

	if (!bIsAttackOnCooldown && (TargetActor && Weapon->IsInRange(GetOwner(), TargetActor)) || TargetLocation != FHConstants::NullVector)
	{
		StartAttack();
	}

	if (TargetActor)
	{
		FollowTargetActor();
	}
}



