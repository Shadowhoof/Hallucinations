#pragma once

#include "CoreMinimal.h"

#include "UObject/Interface.h"

#include "HInteractable.generated.h"

class AHCharacter;


namespace InteractableConstants
{
	const float DefaultInteractionRange = 200.f;
}


UINTERFACE(MinimalAPI)
class UHInteractable : public UInterface
{ 
	GENERATED_BODY()
};

class IHInteractable
{
	GENERATED_BODY()

public:

	virtual void InteractWith(AHCharacter* Interactor) = 0;

	virtual float GetInteractionRange() const;

	virtual FVector GetInteractableLocation() const = 0;

	bool IsInRange(const FVector& QuerierLocation) const;

	UFUNCTION(BlueprintImplementableEvent)
	void OnHoverBegin();

	UFUNCTION(BlueprintImplementableEvent)
	void OnHoverEnd();
	
};
