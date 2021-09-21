#include "StatusEffects/HStatusEffectComponent.h"

#include "Characters/HCharacter.h"
#include "Core/HLogCategories.h"
#include "Kismet/GameplayStatics.h"
#include "StatusEffects/HStatusEffect.h"
#include "Utils/HEnumTools.h"
#include "Particles/ParticleSystemComponent.h"

DEFINE_LOG_CATEGORY(LogStatusEffect);

UHStatusEffectComponent::UHStatusEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHStatusEffectComponent::ApplyEffectFromClass(TSubclassOf<UHStatusEffect> EffectClass, AActor* SourceActor,
	float Duration)
{
	UHStatusEffect* Effect = NewObject<UHStatusEffect>(this, EffectClass);
	ApplyEffect(Effect, SourceActor, Duration);
}

void UHStatusEffectComponent::ApplyEffect(UHStatusEffect* Effect, AActor* SourceActor, float Duration)
{
	ensure(Effect && !Effect->IsActive());
	Effect->Begin(GetOwner(), SourceActor, Duration);
	ActiveEffects.Add(Effect);
}

void UHStatusEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{
	for (UHStatusEffect* Effect : ActiveEffects)
	{
		Effect->Tick(DeltaTime);
	}

	ActiveEffects.RemoveAll(
		[](const UHStatusEffect* Effect)
		{
			return !Effect->IsActive();
		}
	);
}

uint8 UHStatusEffectComponent::GetConditions() const
{
	return Conditions;
}

bool UHStatusEffectComponent::IsConditionActive(EStatusCondition Condition) const
{
	return TEST_BIT(Conditions, Condition);
}

void UHStatusEffectComponent::ApplyCondition(EStatusCondition Condition)
{
	if (IsConditionActive(Condition))
	{
		return;
	}

	SET_BIT(Conditions, Condition);
	ConditionAppliedEvent.Broadcast(Condition);

	if (Condition == EStatusCondition::Stunned && StunnedVFX)
	{
		AHCharacter* Character = Cast<AHCharacter>(GetOwner());
		FVector Offset(0.f, 0.f, Character->GetDefaultHalfHeight());
		ActiveStunnedVFX = UGameplayStatics::SpawnEmitterAttached(StunnedVFX, Character->GetDefaultAttachComponent(), NAME_None, Offset);
	}

	UE_LOG(LogStatusEffect, Log, TEXT("Condition %s applied to %s"), *EnumToString(Condition), *GetOwner()->GetName());
}

void UHStatusEffectComponent::RemoveCondition(EStatusCondition Condition)
{
	if (!IsConditionActive(Condition))
	{
		return;
	}

	for (UHStatusEffect* Effect : ActiveEffects)
	{
		if (TEST_BIT(Effect->GetActiveConditions(), Condition))
		{
			return;
		}
	}

	UNSET_BIT(Conditions, Condition);
	ConditionRemovedEvent.Broadcast(Condition);
	
	UE_LOG(LogStatusEffect, Log, TEXT("Condition %s removed from %s"), *EnumToString(Condition), *GetOwner()->GetName());
}

UHStatusEffectComponent::FConditionAppliedEvent& UHStatusEffectComponent::OnConditionApplied()
{
	return ConditionAppliedEvent;
}

UHStatusEffectComponent::FConditionRemovedEvent& UHStatusEffectComponent::OnConditionRemoved()
{
	return ConditionRemovedEvent;
}

void UHStatusEffectComponent::OnDeath()
{
	for (UHStatusEffect* Effect : ActiveEffects)
	{
		Effect->End();
	}
	ActiveEffects.Empty();

	Conditions = 0;

	if (ActiveStunnedVFX.IsValid())
	{
		ActiveStunnedVFX->DestroyComponent();
	}
}


