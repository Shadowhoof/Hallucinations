// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HHealthComponent.h"
#include "Characters/HCharacter.h"
#include "Core/GameModes/HGameMode.h"
#include "Core/Subsystems/Save/HLevelSave.h"
#include "Perception/AISense_Damage.h"

DEFINE_LOG_CATEGORY(LogRagdoll);

// Sets default values for this component's properties
UHHealthComponent::UHHealthComponent()
{
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;

	Team = ETeam::BadGuys;

	RagdollForce = 10000;
	MaxRagdollForce = 100000;
}

// Called when the game starts
void UHHealthComponent::BeginPlay() {
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	GetOwner()->OnTakePointDamage.AddDynamic(this, &UHHealthComponent::HandleTakePointDamage);
	GetOwner()->OnTakeRadialDamage.AddDynamic(this, &UHHealthComponent::HandleTakeRadialDamage);
}

void UHHealthComponent::ApplyHealthChange(float Delta, const UDamageType* DamageType, AController* Instigator, AActor* Source)
{
	if (Delta == 0.f)
		return;

	if (Delta < 0.f)
	{
		if (CurrentHealth <= 0.f)
			return;

		Delta = -FMath::Min(CurrentHealth, -Delta);
	}
	else
	{
		if (CurrentHealth >= MaxHealth)
			return;

		Delta = FMath::Min(MaxHealth - CurrentHealth, Delta);
	}
	
	CurrentHealth += Delta;
	OnHealthChanged.Broadcast(GetOwner(), Instigator, Source, Delta);

	if (Delta < 0.f)
	{
		// damage has been dealt
		float Damage = -Delta;
		AActor* Victim = GetOwner();
		AActor* InstigatorActor = Instigator ? Instigator->GetPawn() : Source;
		if (InstigatorActor)
		{
			UAISense_Damage::ReportDamageEvent(this, Victim, InstigatorActor, Damage, InstigatorActor->GetActorLocation(), Victim->GetActorLocation());
		}
	}
	
	if (CurrentHealth <= 0.f)
	{
		AHGameMode* GameMode = Cast<AHGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnActorDeath(GetOwner(), Source);
		}
		
		OnActorDeath.Broadcast(GetOwner(), Source);
	}
}

bool UHHealthComponent::AreEnemies(const AActor* FirstActor, const AActor* SecondActor)
{
	return GetThreatStatus(FirstActor, SecondActor) == EThreatStatus::Enemy;
}

bool UHHealthComponent::AreAllies(const AActor* FirstActor, const AActor* SecondActor)
{
	return GetThreatStatus(FirstActor, SecondActor) == EThreatStatus::Ally;
}

bool UHHealthComponent::IsEnemy(const AActor* OtherActor) const
{
	return AreEnemies(this->GetOwner(), OtherActor);
}

EThreatStatus UHHealthComponent::GetThreatStatus(const AActor* FirstActor, const AActor* SecondActor)
{
	if (!FirstActor || !SecondActor)
		return EThreatStatus::Neutral;

	UHHealthComponent* FirstHealthComponent = Cast<UHHealthComponent>(FirstActor->GetComponentByClass(StaticClass()));
	if (!FirstHealthComponent)
		return EThreatStatus::Neutral;

	UHHealthComponent* SecondHealthComponent = Cast<UHHealthComponent>(SecondActor->GetComponentByClass(StaticClass()));
	if (!SecondHealthComponent)
		return EThreatStatus::Neutral;

	return FirstHealthComponent->Team == SecondHealthComponent->Team ? EThreatStatus::Ally : EThreatStatus::Enemy;
}

ETeam UHHealthComponent::GetTeam() const
{
	return Team;
}

void UHHealthComponent::HandleTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy,
	FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	const UDamageType* DamageType, AActor* DamageCauser)
{
	if (Damage <= 0.f) {
		UE_LOG(LogDamage, Warning, TEXT("Negative damage %.2f applied from %s to %s"), Damage, *InstigatedBy->GetName(), *DamagedActor->GetName());
		return;
	}
	UE_LOG(LogDamage, Log, TEXT("Taking point damage from %s, amount: %.0f"), *DamageCauser->GetName(), Damage);

	AActor* InstigatorActor = InstigatedBy ? InstigatedBy->GetCharacter() : nullptr;
	OnDamageTaken.Broadcast(DamagedActor, InstigatorActor, Damage);
	
	const bool bWasAlive = !IsDead();
	ApplyHealthChange(-Damage, DamageType, InstigatedBy, DamageCauser);
	if (bWasAlive && IsDead())
	{
		AHCharacter* Character = Cast<AHCharacter>(GetOwner());
		const float ActualRagdollForce = FMath::Min(Damage * RagdollForce, MaxRagdollForce);
		Character->GetMesh()->AddImpulseAtLocation(ShotFromDirection * ActualRagdollForce, HitLocation, BoneName);
		UE_LOG(LogRagdoll, Verbose, TEXT("Applied %.0f force to ragdoll %s"), ActualRagdollForce, *DamagedActor->GetName());
	}
}

void UHHealthComponent::HandleTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
	FVector Origin, FHitResult HitInfo, AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.f) {
		UE_LOG(LogDamage, Warning, TEXT("Negative damage %.2f applied from %s to %s"), Damage, *InstigatedBy->GetName(), *DamagedActor->GetName());
		return;
	}
	UE_LOG(LogDamage, Log, TEXT("Taking radial damage from %s, amount: %.0f"), *DamageCauser->GetName(), Damage);

	AActor* InstigatorActor = InstigatedBy ? InstigatedBy->GetCharacter() : nullptr;
	OnDamageTaken.Broadcast(DamagedActor, InstigatorActor, Damage);
	ApplyHealthChange(-Damage, DamageType, InstigatedBy, DamageCauser);
}

float UHHealthComponent::GetHealthPercentage() const
{
	return FMath::Clamp(CurrentHealth / MaxHealth, 0.f, 1.f);
}

void UHHealthComponent::RestorePersistentState(float HealthPercentage)
{
	CurrentHealth = MaxHealth * FMath::Clamp(HealthPercentage, 0.f, 1.f); 
}

bool UHHealthComponent::IsDead() const
{
	return CurrentHealth <= 0.f;
}


