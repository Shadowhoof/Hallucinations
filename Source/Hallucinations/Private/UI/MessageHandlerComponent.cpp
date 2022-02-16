// Created by Dmitrii Korovin


#include "UI/MessageHandlerComponent.h"

#include "Abilities/HAbilityComponent.h"
#include "Components/HResourceComponent.h"
#include "Constants/HConstants.h"
#include "Internationalization/StringTableRegistry.h"

namespace GameplayEventKeys
{
	const FString OutOfMana = "OutOfMana";
	const FString AbilityOnCooldown = "AbilityNotReady";
}


UMessageHandlerComponent::UMessageHandlerComponent()
{
}

void UMessageHandlerComponent::SetCharacter(AHPlayerCharacter* InCharacter)
{
	Character = InCharacter;

	Character->GetAbilityComponent()->AbilityOnCooldownEvent.AddUObject(this, &UMessageHandlerComponent::OnAbilityOnCooldown);
	Character->GetResourceComponent()->OutOfManaEvent.AddUObject(this, &UMessageHandlerComponent::OnOutOfMana);
}

void UMessageHandlerComponent::OnAbilityOnCooldown()
{
	GameplayMessageEvent.Broadcast(GetGameplayEventMessage(GameplayEventKeys::AbilityOnCooldown));
}

void UMessageHandlerComponent::OnOutOfMana()
{
	GameplayMessageEvent.Broadcast(GetGameplayEventMessage(GameplayEventKeys::OutOfMana));
}

FText UMessageHandlerComponent::GetGameplayEventMessage(const FString& Key)
{
	return FText::FromStringTable(DataConstants::GameplayEventStringTable, Key);
}

