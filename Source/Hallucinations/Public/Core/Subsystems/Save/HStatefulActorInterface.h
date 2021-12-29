// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "HLevelSave.h"
#include "UObject/Interface.h"
#include "HStatefulActorInterface.generated.h"


UINTERFACE()
class UHStatefulActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * This interface should be implemented by persistent actors in a level. When level changes then all saved actors
 * implementing this interface are saved and will be restored if that level is entered again during the same playthrough. 
 */
class HALLUCINATIONS_API IHStatefulActorInterface
{
	GENERATED_BODY()

public:

	/** Indicates whether current actor should be included in saved level state */
	virtual bool ShouldBeSaved() const;

	/** Returns persistent saved of this actor which will be saved as a part of level state */
	virtual FPersistentActorState GetPersistentState() const;

	/** Restores actor state from a previously saved data */
	virtual void RestorePersistentState(const FPersistentActorState& State);
};
