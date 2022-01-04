// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HStatefulActorInterface.generated.h"


USTRUCT()
struct FPersistentActorState
{
	GENERATED_BODY()

	/** Class which will be used to recreate an actor */ 
	UPROPERTY()
	TSubclassOf<AActor> Class;

	/** Actor's transform */
	UPROPERTY()
	FTransform Transform;

	/** Actor's health */
	UPROPERTY()
	float Health;

	/** Ability cooldowns of an actor at the moment of save */
	UPROPERTY()
	TMap<FString, float> AbilityCooldowns;
	
};


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
