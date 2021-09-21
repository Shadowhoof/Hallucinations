// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HStatusEffectComponent.generated.h"

class UHStatusEffect;

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EStatusCondition : uint8
{
	None = 0 UMETA(Hidden),
	Stunned = 1,
	Chilled = 2,
	Frozen = 3,
};
ENUM_CLASS_FLAGS(EStatusCondition)


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UHStatusEffectComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UHStatusEffectComponent();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Conditions")
	uint8 Conditions = 0;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Effects")
	TArray<UHStatusEffect*> ActiveEffects;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	UParticleSystem* StunnedVFX;

	UPROPERTY()
	TWeakObjectPtr<UParticleSystemComponent> ActiveStunnedVFX;

public:

	UFUNCTION(BlueprintCallable, Category = "Effects")
	void ApplyEffectFromClass(TSubclassOf<UHStatusEffect> EffectClass, AActor* SourceActor, float Duration);

	UFUNCTION(BlueprintCallable, Category = "Effects")
	void ApplyEffect(UHStatusEffect* Effect, AActor* SourceActor, float Duration);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Conditions")
	uint8 GetConditions() const;

	UFUNCTION(BlueprintCallable, Category = "Conditions")
	bool IsConditionActive(EStatusCondition Condition) const;

	void ApplyCondition(EStatusCondition Condition);

	void RemoveCondition(EStatusCondition Condition);

	DECLARE_EVENT_OneParam(UHStatusEffectComponent, FConditionAppliedEvent, EStatusCondition)
	FConditionAppliedEvent& OnConditionApplied();

	DECLARE_EVENT_OneParam(UHStatusEffectComponent, FConditionRemovedEvent, EStatusCondition)
	FConditionRemovedEvent& OnConditionRemoved();

	void OnDeath();

private:

	FConditionAppliedEvent ConditionAppliedEvent;
	FConditionRemovedEvent ConditionRemovedEvent;
};
