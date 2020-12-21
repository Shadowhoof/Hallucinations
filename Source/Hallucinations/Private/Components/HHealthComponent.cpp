// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HHealthComponent.h"
#include "Characters/HCharacter.h"

DEFINE_LOG_CATEGORY(LogRagdoll);

// Sets default values for this component's properties
UHHealthComponent::UHHealthComponent()
{
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;

	Team = ETeam::BadGuys;

	RagdollForce = 10000;
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
	OnHealthChanged.Broadcast(GetOwner(), Source, Delta);
	UE_LOG(LogTemp, Log, TEXT("Actor %s health changed, delta: %.2f, remaining health: %.2f"), *GetOwner()->GetName(), Delta, CurrentHealth);
	if (CurrentHealth <= 0.f)
	{
		OnActorDeath.Broadcast(GetOwner(), Source);
		UE_LOG(LogTemp, Log, TEXT("Actor %s died, killer is %s"), *GetOwner()->GetName(), *Instigator->GetPawn()->GetName())
	}
}

bool UHHealthComponent::AreEnemies(AActor* FirstActor, AActor* SecondActor)
{
	return GetThreatStatus(FirstActor, SecondActor) == EThreatStatus::Enemy;
}

bool UHHealthComponent::AreAllies(AActor* FirstActor, AActor* SecondActor)
{
	return GetThreatStatus(FirstActor, SecondActor) == EThreatStatus::Ally;
}

bool UHHealthComponent::IsEnemy(AActor* OtherActor) const
{
	return AreEnemies(this->GetOwner(), OtherActor);
}

EThreatStatus UHHealthComponent::GetThreatStatus(AActor* FirstActor, AActor* SecondActor)
{
	if (!FirstActor || !SecondActor)
		return EThreatStatus::Invalid;

	UHHealthComponent* FirstHealthComponent = Cast<UHHealthComponent>(FirstActor->GetComponentByClass(StaticClass()));
	if (!FirstHealthComponent)
		return EThreatStatus::Invalid;

	UHHealthComponent* SecondHealthComponent = Cast<UHHealthComponent>(SecondActor->GetComponentByClass(StaticClass()));
	if (!SecondHealthComponent)
		return EThreatStatus::Invalid;

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

	const bool bWasAlive = !IsDead();
	ApplyHealthChange(-Damage, DamageType, InstigatedBy, DamageCauser);
	if (bWasAlive && IsDead())
	{
		AHCharacter* Character = Cast<AHCharacter>(GetOwner());
		Character->GetMesh()->AddImpulseAtLocation(ShotFromDirection * Damage * RagdollForce, HitLocation, BoneName);
		UE_LOG(LogRagdoll, Log, TEXT("Applied %.0f force to ragdoll %s"), Damage * RagdollForce, *GetName());
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

	ApplyHealthChange(-Damage, DamageType, InstigatedBy, DamageCauser);
}

float UHHealthComponent::GetHealthPercentage() const
{
	return FMath::Clamp(CurrentHealth / MaxHealth, 0.f, 1.f);
}

bool UHHealthComponent::IsDead() const
{
	return CurrentHealth <= 0.f;
}


