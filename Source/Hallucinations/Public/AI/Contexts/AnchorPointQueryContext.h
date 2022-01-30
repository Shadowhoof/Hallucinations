// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryContext.h"
#include "AnchorPointQueryContext.generated.h"

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API UAnchorPointQueryContext : public UEnvQueryContext
{
	GENERATED_BODY()

	virtual void ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const override;
};
