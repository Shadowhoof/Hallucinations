// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HAttackComponent.h"
#include "Components/HHealthComponent.h"
#include "Components/HFollowComponent.h"
#include "Weapons/HWeapon.h"
#include "Characters/HCharacter.h"
#include "Utils/HUtils.h"
#include "HConstants.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UHAttackComponent::UHAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	AttackDelay = 1.f;
	MaxAttacksPerSecond = 1.f;
	BaseAttackSpeed = 1.f;

	TargetActor = nullptr;
	TargetLocation = FHConstants::Null_Vector;
	
	bIsAttacking = false;
	bIsAttackOnCooldown = false;
}

// Called when the game starts
void UHAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponClass) 
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Weapon = GetWorld()->SpawnActor<AHWeapon>(WeaponClass, SpawnParams);

		if (Weapon)
		{
			AHCharacter* Character = Cast<AHCharacter>(GetOwner());
			Weapon->SetOwner(Character);

			const FName SocketName = Weapon->GetAttachmentSocketName();
			if (Character && !SocketName.IsNone())
			{
				Weapon->AttachToComponent(Character->GetMesh(),
				                          FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				                          Weapon->GetAttachmentSocketName());
			}

			UAnimMontage* AttackAnimation = Weapon->GetAttackAnimation();
			if (AttackAnimation) 
			{
				AttackDelay = AttackAnimation->GetSectionLength(0);
				MaxAttacksPerSecond = 1.f / BaseAttackSpeed;
			}
		}
	}
}

bool UHAttackComponent::AttackActor(AActor* Actor) {
	if (Actor == GetOwner() || bIsAttacking || !UHUtils::AreEnemies(GetOwner(), Actor))
		return false;

	TargetActor = Actor;
	TargetLocation = FHConstants::Null_Vector;
	AttackMode = EAttackMode::LockedActor;

	const bool bIsInRange = Weapon->IsInRange(GetOwner(), TargetActor);
	UHFollowComponent* FollowComponent = Cast<UHFollowComponent>(GetOwner()->GetComponentByClass(UHFollowComponent::StaticClass()));
	if (FollowComponent)
	{
		if (bIsInRange)
		{
			FollowComponent->RotateTowards(TargetActor);
		}
		else
		{
			FollowComponent->MoveTo(TargetActor);
		}
	}
	
	if (bIsInRange)
	{
		StartAttack();
	}

	return bIsInRange;
}

void UHAttackComponent::AttackLocation(const FVector& Location)
{
	if (bIsAttacking)
	{
		return;
	}

	TargetLocation = Location;
	TargetActor = nullptr;
	AttackMode = EAttackMode::Ground;

	UHFollowComponent* FollowComponent = Cast<UHFollowComponent>(GetOwner()->GetComponentByClass(UHFollowComponent::StaticClass()));
	if (FollowComponent)
	{
		FollowComponent->RotateTowards(Location);
	}
	
	StartAttack();
}

void UHAttackComponent::StopAttacking()
{
	if (!bIsAttacking)
	{
		TargetLocation = FHConstants::Null_Vector;
		TargetActor = nullptr;
		AttackMode = EAttackMode::None;
		bHasAttackedWhileLocked = false;
	}
}

void UHAttackComponent::CancelActorLock()
{
	if (AttackMode == EAttackMode::LockedActor)
	{
		if (!bHasAttackedWhileLocked)
		{
			AttackMode = EAttackMode::Actor;
		}
		else
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
	if (!MouseoverData.bBlockingHit)
	{
		return;
	}
	
	if (bIsGroundAttack)
	{
		AttackLocation(MouseoverData.ImpactPoint);
	}
	else if (!bIsRepeated)
	{
		AActor* MouseoverActor = MouseoverData.GetActor();
		if (CanBeAttacked(GetOwner(), MouseoverActor))
		{
			AttackActor(MouseoverActor);
		}
	}
}

EAttackMode UHAttackComponent::GetCurrentAttackMode() const
{
	return AttackMode;
}

void UHAttackComponent::StartAttack()
{
	if (!Weapon || bIsAttacking || bIsAttackOnCooldown)
	{
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &UHAttackComponent::PerformAttack, AttackDelay);
	bIsAttacking = true;

	UHFollowComponent* FollowComponent = Cast<UHFollowComponent>(GetOwner()->GetComponentByClass(UHFollowComponent::StaticClass()));
	if (FollowComponent)
	{
		FollowComponent->LockMovement();
	}
	AttackStartedEvent.Broadcast();
}

void UHAttackComponent::PerformAttack()
{
	ensure(TargetLocation != FHConstants::Null_Vector || TargetActor);

	bIsAttacking = false;

	switch (AttackMode)
	{
	case EAttackMode::Actor:
		Weapon->AttackActor(TargetActor);
		StopAttacking();
		break;
	case EAttackMode::LockedActor:
		Weapon->AttackActor(TargetActor);
		bHasAttackedWhileLocked = true;
		break;
	case EAttackMode::Ground:
		Weapon->AttackLocation(TargetLocation);
		TargetLocation = FHConstants::Null_Vector;
		break;
	case EAttackMode::None:
		break;
	}

	const float AttackCooldown = BaseAttackSpeed - AttackDelay;
	if (AttackCooldown > 0.0)
	{
		GetWorld()->GetTimerManager().SetTimer(AttackCooldownTimerHandle, this,
		                                       &UHAttackComponent::OnAttackCooldownOver, AttackCooldown);
		bIsAttackOnCooldown = true;
	}

	UHFollowComponent* FollowComponent = Cast<UHFollowComponent>(GetOwner()->GetComponentByClass(UHFollowComponent::StaticClass()));
	if (FollowComponent) {
		FollowComponent->UnlockMovement();
	}

	AttackEndedEvent.Broadcast();
}

void UHAttackComponent::OnAttackCooldownOver()
{
	bIsAttackOnCooldown = false;
}

// Called every frame
void UHAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bIsAttacking || bIsAttackOnCooldown)
	{
		return;
	}

	if (TargetActor && Weapon->IsInRange(GetOwner(), TargetActor) || TargetLocation != FHConstants::Null_Vector)
	{
		StartAttack();
	}
}



