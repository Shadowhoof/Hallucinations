#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "Components/HAttackComponent.h"
#include "HAbilityComponent.generated.h"

class UHSpellAbility;
class UHPlayerCharacterSave;
class AHCharacter;
class UHAbility;
class UHAttackAbility;
class UHPlayerCharacterSave;


USTRUCT()
struct FAbilityTargetParameters
{
	GENERATED_BODY()

	// target actor
	TWeakObjectPtr<AActor> Actor;

	// target location
	FVector Location;
};


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HALLUCINATIONS_API UHAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UHAbilityComponent();

	UFUNCTION(BlueprintCallable)
	void UseAbility(UHAbility* Ability);

	AActor* GetTargetActor() const;

	FVector GetTargetLocation() const;

	AHCharacter* GetCaster() const;

	bool IsCasting() const;

	void Interrupt();

	TArray<UHAbility*> GetAbilities() const;

	bool HasAbility(UHAbility* Ability) const;

	void GetPersistentState(TMap<FString, float>& OutCooldownData);
	void RestorePersistentState(const TMap<FString, float>& CooldownData);

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UHAbility>> AvailableAbilities;

	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	TArray<UHAbility*> Abilities;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DefaultCastAnimation;
	
	FTimerHandle CastTimerHandle;

	bool bIsCasting = false;

	virtual void BeginPlay() override;

	/**
	 * Returns whether provided ability is allowed to be used by the component. Things like stun, silence, etc. are checked here.
	 * Also queries ability if it's ready to be used (i.e. if it's off cooldown).
	 */
	bool CanUseAbility(UHAbility* Ability, const FAbilityTargetParameters& TargetParams) const;

	void FinishCast(UHSpellAbility* Ability);

private:

	void UseSpellAbility(UHAbility* BaseAbility);
	void UseAttackAbility(UHAbility* UncastAbility);

	FAbilityTargetParameters CurrentTargetParams;

	UPROPERTY()
	UHAttackAbility* QueuedAttackAbility;

	UFUNCTION()
	void OnAttackEnded(const FAttackResult& AttackResult);

	float GetCastTime(UHSpellAbility* Ability) const;

	UAnimMontage* GetCastAnimation(UHSpellAbility* Ability) const;

	// constants
	static const float ChilledCastTimeMultiplier;
	
};


/**
 * Action bar of a player character
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HALLUCINATIONS_API UHActionBarComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UHActionBarComponent();

	UFUNCTION(BlueprintCallable)
	void UseAbilityByIndex(int32 Index);

	/**
	 * Assigns provided ability to an action bar slot. Can be nullptr to remove ability from action bar.
	 * @return whether operation was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "ActionBar")
	bool SetActionBarAbility(UHAbility* Ability, int32 Index);

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	
protected:

	UPROPERTY(BlueprintReadOnly, Category = "ActionBar")
	TArray<UHAbility*> EquippedAbilities;

	UPROPERTY()
	TWeakObjectPtr<UHAbilityComponent> AbilityComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActionBar", meta = (ClampMin = 0))
	int32 MaxAbilities = 4;

	/** Returns ability based on provided action bar index */
	UHAbility* GetAbilityByIndex(int32 Index) const;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:

	/** Saves action bar to a save file */
	void SaveActionBar();

	/** Populates action bar with some abilities if save file was not found */
	void SetDefaultActionBar(const TArray<UHAbility*>& AllAbilities);
	
	/** Loads action bar from a save file */
	void LoadActionBar(TArray<UHAbility*>& AllAbilities, const TArray<FString>& SavedAbilities);
	
};