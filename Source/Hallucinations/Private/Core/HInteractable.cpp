#include "Core/HInteractable.h"

#include "Utils/HLogUtils.h"

bool IHInteractable::IsInRange(const FVector& QuerierLocation) const
{
	float Distance = FVector::Distance(QuerierLocation, GetInteractableLocation());
	return Distance <= GetInteractionRange();
}
