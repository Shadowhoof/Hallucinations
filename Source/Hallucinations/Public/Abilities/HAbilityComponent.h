﻿#pragma once

#include "CoreMinimal.h"

#include "HConstants.h"
#include "Components/ActorComponent.h"
#include "HAbilityComponent.generated.h"

class AHCharacter;
class UHAbility;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class HALLUCINATIONS_API UHAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UHAbilityComponent();

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	uint8 MaxAbilities = 4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UHAbility>> AvailableAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Abilities")
	TArray<TSubclassOf<UHAbility>> SelectedAbilities;

	UPROPERTY(BlueprintReadOnly, Category = "Abilities")
	TArray<UHAbility*> Abilities;

	FTimerHandle CastTimerHandle;

	bool bIsCasting = false;
	
	virtual void BeginPlay() override;

	bool CanUseAbility(uint8 Index) const;
	
	void FinishCast();

	FTimerDelegate CastCallback;
	
public:

	UFUNCTION(BlueprintCallable)
	void UseAbility(uint8 Index);

	AActor* GetTargetActor() const;

	FVector GetTargetLocation() const;

	AHCharacter* GetCaster() const;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	void StartCast(float CastTime, const FTimerDelegate& Delegate);
	
	bool IsCasting() const;

	void Interrupt();
};
