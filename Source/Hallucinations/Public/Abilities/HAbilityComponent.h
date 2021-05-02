#pragma once

#include "CoreMinimal.h"
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

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	uint8 MaxAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UHAbility>> AvailableAbilities;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UHAbility>> SelectedAbilities;

	UPROPERTY()
	TArray<UHAbility*> Abilities;

	virtual void BeginPlay() override;

	bool CanUseAbility(uint8 Index) const;

public:

	void UseAbility(uint8 Index);

	AActor* GetTargetActor() const;

	FVector GetTargetLocation() const;

	AHCharacter* GetCaster() const;
};
