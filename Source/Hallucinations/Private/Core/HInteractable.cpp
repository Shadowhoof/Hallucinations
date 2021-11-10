#include "Core/HInteractable.h"

float IHInteractable::GetInteractionRange() const
{
	return InteractableConstants::DefaultInteractionRange;
}

bool IHInteractable::IsInRange(const FVector& QuerierLocation) const
{
	float Distance = FVector::Distance(QuerierLocation, GetInteractableLocation());
	return Distance <= GetInteractionRange();
}
