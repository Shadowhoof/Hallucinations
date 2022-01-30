// Created by Dmitrii Korovin


#include "AI/Contexts/AnchorPointQueryContext.h"

#include "Characters/HNonPlayerCharacter.h"
#include "Controllers/HAIController.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Point.h"

void UAnchorPointQueryContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	if (const AHNonPlayerCharacter* Character = Cast<AHNonPlayerCharacter>(QueryInstance.Owner.Get()))
	{
		UEnvQueryItemType_Point::SetContextHelper(ContextData, Character->GetAnchorPoint());
	}
}
