// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "HFadeable.generated.h"

/**
 *  Interface for objects which can become transparent to a certain degree (fade) on demand
 */
UINTERFACE(MinimalAPI)
class UHFadeable : public UInterface
{
	GENERATED_BODY()
};

class IHFadeable
{
	GENERATED_BODY()

public:

	virtual void FadeOut();

	virtual void FadeIn();
	
};
