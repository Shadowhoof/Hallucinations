// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StatusEffects/HStatusEffectComponent.h"
#include "HStatusEffect.generated.h"

class UHStatusEffectComponent;

/**
 * A buff or a debuff that can be applied to an actor with HStatusEffectComponent. 
 */
UCLASS()
class HALLUCINATIONS_API UHStatusEffect : public UObject
{
	GENERATED_BODY()

protected:

	/* Actor affected by the status effect */
	UPROPERTY(BlueprintReadOnly)
	AActor* AffectedActor;

	/* Actor who cast the status effect */
	UPROPERTY(BlueprintReadOnly)
	AActor* SourceActor;

	UPROPERTY(BlueprintReadOnly)
	float InitialDuration;

	UPROPERTY(BlueprintReadOnly)
	float RemainingDuration;

	bool bIsActive = false;

	/* Returns StatusEffectComponent of affected actor */
	UHStatusEffectComponent* GetComponent() const;
	
public:
	
	/* Called when status effect is applied */
	virtual void Begin(AActor* Affected, AActor* Source, float Duration);

	/* Called when status effect fades */
	virtual void End();

	virtual void Tick(float DeltaTime);

	float GetRemainingDuration() const;
	
	float GetRemainingPercentage() const;

	virtual uint8 GetActiveConditions() const;

	bool IsActive() const;
	
};
