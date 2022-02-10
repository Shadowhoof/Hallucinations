// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "HAbilityComponent.h"
#include "UObject/NoExportTypes.h"
#include "HAbility.generated.h"

class UAbilityInstance;
enum class EAbilityEffectTrigger : uint8;
class UHAbilityComponent;
class AHAbilityImpl;
class UAnimMontage;
class UTexture2D;
class UAbilityEffect;

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EAbilityTarget : uint8
{
	None = 0 UMETA(Hidden),
	Self = 1 << 0,
	Actor = 1 << 1,
	Point = 1 << 2
};
ENUM_CLASS_FLAGS(EAbilityTarget)


UENUM(BlueprintType)
enum class EAbilityType : uint8
{
	// uses attack logic for executing, applies its effect on attack
	Attack,
	// uses casting logic for executing
	Spell
};


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
	
public:

	UHAbility();
	
	/** Returns ability name if it is specified, otherwise returns class name */
	UFUNCTION(BlueprintCallable)
	FText GetSkillName() const;

	const FString& GetSkillNameAsString() const;

	/**
	 * Returns whether current ability can be used with provided target parameters. For example if this ability only
	 * supports actor targeting then a valid actor is required to be specified in target parameters.
	 */
	bool CanBeUsed(const FAbilityTargetParameters& TargetParams) const;

	EAbilityType GetType() const;

	EAbilityTarget GetTargetType(const FAbilityTargetParameters& TargetParams) const;

	float GetManaCost() const;
	
	void SetAbilityComponent(UHAbilityComponent* Component);

	/* Gets remaining cooldown for current ability in seconds */
	UFUNCTION(BlueprintPure, Category = "Cooldown")
	float GetRemainingCooldown() const;

	/* Gets remaining cooldown percentage for current ability (in 0..1 range) */
	UFUNCTION(BlueprintPure, Category = "Cooldown")
	float GetRemainingCooldownPercentage() const;

	void RestoreCooldownPercentage(float CooldownPercentage);

	bool IsOffensive() const;

	const TArray<UAbilityEffect*>& GetEffects() const;
	
protected:

	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UHAbilityComponent* AbilityComponent;
	
	/** Target types which are supported by current ability. @see EAbilityTarget */
	UPROPERTY(EditDefaultsOnly, Category = "Ability", meta = (Bitmask, BitmaskEnum = "EAbilityTarget"))
	uint8 TargetType = 0;

	/** Usage type of ability (can be a spell or an attack) */
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	EAbilityType Type = EAbilityType::Spell;

	UPROPERTY(EditDefaultsOnly, Category = "Ability", meta = (Bitmask, BitmaskEnum = "EThreatStatus"))
	uint8 AffectedTargets;
	
	/** Amount of time before ability can be used again */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float Cooldown = 0.f;

	/** Amount of mana required to use the ability */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	float ManaCost = 10.f;
	
	/** Icon to be displayed in the UI */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Icon;

	/** Skill display name. Can be empty. Use GetSkillName if non-empty value is required */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	FText Name = FText::GetEmpty();

	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category = "Ability")
	TArray<UAbilityEffect*> Effects; 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TSubclassOf<UAbilityInstance> InstanceClass; 

	FTimerHandle CooldownTimerHandle;

	virtual bool IsTargetTypeValid(const FAbilityTargetParameters& TargetParams) const;

	void StartCooldown();
	
	void CreateInstance(const FTransform& SpawnTransform, AActor* TargetActor);
	void CreateInstance(const FTransform& SpawnTransform, const FVector& TargetLocation);

private:

	void CreateInstanceImpl(const FTransform& SpawnTransform, AActor* TargetActor, const FVector& TargetLocation);
};
