// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "HResourceComponent.generated.h"

/**
 *  Component that contains various character resources (i.e. mana)
 */
UCLASS()
class HALLUCINATIONS_API UHResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UHResourceComponent();

	/** Returns current raw amount of mana available */
	float GetCurrentMana() const;

	/** Returns current percentage of mana available ranging from 0.0 to 1.0 */ 
	UFUNCTION(BlueprintCallable)
	float GetManaPercentage() const;

	/**
	 *	Request to spend some amount of mana
	 *	@return true if mana was spent, false otherwise
	 */
	bool SpendMana(float Amount);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void RestorePersistentState(const float ManaPercentage);

	DECLARE_EVENT(UHResourceComponent, FOutOfManaEvent)
	FOutOfManaEvent OutOfManaEvent;
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mana")
	float MaxMana = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mana")
	float CurrentMana = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mana")
	float ManaRegeneration = 5.f;
	
};
