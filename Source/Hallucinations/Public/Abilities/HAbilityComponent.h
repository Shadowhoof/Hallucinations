#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"
#include "HAbilityComponent.generated.h"

class UHSaveGame;
class AHCharacter;
class UHAbility;
class UHSaveGame;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HALLUCINATIONS_API UHAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UHAbilityComponent();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UHAbility>> AvailableAbilities;

	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	TArray<UHAbility*> Abilities;
	
	FTimerHandle CastTimerHandle;

	bool bIsCasting = false;
	
	virtual void BeginPlay() override;

	/** Returns whether provided ability is allowed to be used by the component. Things like stun, silence, etc. are checked here. \
	 * Things that are controlled by ability itself (like cooldown) aren't checked here. */
	bool CanUseAbility(UHAbility* Ability) const;

	void FinishCast();

	FTimerDelegate CastCallback;
	
public:

	UFUNCTION(BlueprintCallable)
	void UseAbility(UHAbility* Ability);

	AActor* GetTargetActor() const;

	FVector GetTargetLocation() const;

	AHCharacter* GetCaster() const;

	void StartCast(float CastTime, const FTimerDelegate& Delegate);
	
	bool IsCasting() const;

	void Interrupt();

	TArray<UHAbility*> GetAbilities() const;

	bool HasAbility(UHAbility* Ability);
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

protected:

	UPROPERTY(BlueprintReadOnly, Category = "ActionBar")
	TArray<UHAbility*> EquippedAbilities;

	UPROPERTY()
	TWeakObjectPtr<UHAbilityComponent> AbilityComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ActionBar")
	uint8 MaxAbilities = 4;

	/** Returns ability based on provided action bar index */
	UHAbility* GetAbilityByIndex(uint8 Index) const;

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Populates action bar with some abilities if save file wasn't found */
	void SetDefaultActionBar(const TArray<UHAbility*>& AllAbilities);

	/** Loads action bar from a save file */
	void LoadActionBar(TArray<UHAbility*>& AllAbilities, const TArray<FString>& SavedAbilities);

public:
	
	UFUNCTION(BlueprintCallable)
	void UseAbilityByIndex(uint8 Index);

	/**
	 * Assigns provided ability to an action bar slot. Can be nullptr to remove ability from action bar.
	 * @return whether operation was successful
	 */
	UFUNCTION(BlueprintCallable, Category = "ActionBar")
	bool SetActionBarAbility(UHAbility* Ability, uint8 Index);

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

private:

	static const FName EmptyAbilityName;
	
};