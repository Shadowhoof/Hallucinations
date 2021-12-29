// Created by Dmitrii Korovin


#include "Core/Subsystems/Save/HStatefulActorInterface.h"

bool IHStatefulActorInterface::ShouldBeSaved() const
{
	return false;
}

FPersistentActorState IHStatefulActorInterface::GetPersistentState() const
{
	return FPersistentActorState();
}

void IHStatefulActorInterface::RestorePersistentState(const FPersistentActorState& State)
{
}
