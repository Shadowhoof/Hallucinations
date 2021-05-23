// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "HAbilityComponent.h"
#include "UObject/NoExportTypes.h"
#include "HAbility.generated.h"

class UHAbilityComponent;
class AHAbilityImpl;
class UAnimMontage;
class UTexture2D;

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EAbilityTarget : uint8
{
	None = 0 UMETA(Hidden),
	Self = 1 << 0,
	Actor = 1 << 1,
	Point = 1 << 2
};
ENUM_CLASS_FLAGS(EAbilityTarget)

/**
 * A shell which contains information that is required to use the ability (target type, cooldown) and to display
 * ability effects to the player (damage, area of effect, etc.).
 * Does not perform any actions in the world by itself. Rather when Use() method is called then a corresponding
 * actor is created to perform the appropriate actions (@see HAbilityImpl.h)
 */
UCLASS(Blueprintable)
class HALLUCINATIONS_API UHAbility : public UObject
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, meta = (Bitmask, BitmaskEnum = "EAbilityTarget"))
	uint8 TargetType = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Behaviour")
	float Cooldown = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* CastAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Icon;

	FTimerHandle CooldownTimerHandle;

	FTimerHandle CastTimerHandle;

	bool bIsCasting = false;

	void OnCastStarted(AHCharacter* Caster);
	void OnCastFinished();

	/** Use current ability on target actor */
	virtual void Use(UHAbilityComponent* Context, AActor* TargetActor);

	/** Use current ability at target location */
	virtual void Use(UHAbilityComponent* Context, FVector TargetLocation);

	/** Use current ability on owner */
	virtual void Use(UHAbilityComponent* Context);

	void DelayCast(UHAbilityComponent* Context, const FTimerDelegate& Delegate);

	UFUNCTION()
	virtual void FinishActorCast(UHAbilityComponent* Context, AActor* TargetActor);

	UFUNCTION()
	virtual void FinishLocationCast(UHAbilityComponent* Context, FVector TargetLocation);

	UFUNCTION()
	virtual void FinishSelfCast(UHAbilityComponent* Context);

public:

	bool TryUse(UHAbilityComponent* Context);

};