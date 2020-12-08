// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChanged, AActor*, Target, AActor*, Source, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnActorDeath, AActor*, Victim, AActor*, Killer);

UENUM()
enum class ETeam
{
	GoodGuys,
	BadGuys,
};

UENUM()
enum class EThreatStatus
{
	Ally,
	Enemy,
	Invalid,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HALLUCINATIONS_API UHHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHHealthComponent();

private:

	void ApplyHealthChange(AActor* Target, float Delta, const UDamageType* DamageType, AController* Instigator, AActor* Source);
	
protected:

	UPROPERTY(EditAnywhere, Category = "Health")
	float MaxHealth;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	ETeam Team;
	
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:

	UFUNCTION(BlueprintCallable)
	static bool AreEnemies(AActor* FirstActor, AActor* SecondActor);

	bool IsEnemy(AActor* OtherActor) const;

	static EThreatStatus GetThreatStatus(AActor* FirstActor, AActor* SecondActor);

	ETeam GetTeam() const;

	bool IsDead() const;
	
	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnActorDeath OnActorDeath;

	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercentage() const;
};
