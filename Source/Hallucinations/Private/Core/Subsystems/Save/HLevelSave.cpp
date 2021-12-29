// Created by Dmitrii Korovin


#include "Core/Subsystems/Save/HLevelSave.h"


void UHLevelSave::SaveActorState(FPersistentActorState State)
{
	Actors.Add(State);
}

const TArray<FPersistentActorState>& UHLevelSave::GetActorData()
{
	return Actors;
}
