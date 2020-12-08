// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HHealthComponent.h"

// Sets default values for this component's properties
UHHealthComponent::UHHealthComponent()
{
	MaxHealth = 100.f;
	CurrentHealth = MaxHealth;

	Team = ETeam::BadGuys;
}

// Called when the game starts
void UHHealthComponent::BeginPlay() {
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UHHealthComponent::HandleTakeAnyDamage);
}

void UHHealthComponent::ApplyHealthChange(AActor* Target, float Delta, const UDamageType* DamageType, AController* Instigator, AActor* Source)
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

void UHHealthComponent::HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser)
{
	if (Damage <= 0.f)
	{
		UE_LOG(LogDamage, Warning, TEXT("Negative damage %.2f applied from %s to %s"), Damage, *Instigator->GetName(), *DamagedActor->GetName());
		return;
	}
	
	ApplyHealthChange(DamagedActor, -Damage, DamageType, Instigator, DamageCauser);
}

float UHHealthComponent::GetHealthPercentage() const
{
	return FMath::Clamp(CurrentHealth / MaxHealth, 0.f, 1.f);
}

bool UHHealthComponent::IsDead() const
{
	return CurrentHealth <= 0.f;
}


