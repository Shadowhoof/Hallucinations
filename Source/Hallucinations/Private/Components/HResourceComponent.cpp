// Created by Dmitrii Korovin


#include "Components/HResourceComponent.h"

UHResourceComponent::UHResourceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

float UHResourceComponent::GetCurrentMana() const
{
	return CurrentMana;
}

float UHResourceComponent::GetManaPercentage() const
{
	if (MaxMana <= 0.f)
	{
		return 0.f;
	}

	return CurrentMana / MaxMana;
}

bool UHResourceComponent::SpendMana(const float Amount)
{
	if (Amount > CurrentMana)
	{
		OutOfManaEvent.Broadcast();
		return false;
	}

	CurrentMana -= Amount;
	return true;
}

void UHResourceComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float RegeneratedAmount = ManaRegeneration * DeltaTime;  
	CurrentMana = FMath::Min(CurrentMana + RegeneratedAmount, MaxMana);
}

void UHResourceComponent::RestorePersistentState(const float ManaPercentage)
{
	CurrentMana = MaxMana * ManaPercentage;
}
