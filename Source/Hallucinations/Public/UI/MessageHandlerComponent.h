// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "Characters/HPlayerCharacter.h"
#include "Components/ActorComponent.h"
#include "MessageHandlerComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGameplayMessageEvent, FText, Message);


/** PlayerController component that will listen to gameplay events and forward them to UI widgets that display messages. */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HALLUCINATIONS_API UMessageHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UMessageHandlerComponent();

	void SetCharacter(AHPlayerCharacter* InCharacter);

	UPROPERTY(BlueprintAssignable, Category = "Message")
	FGameplayMessageEvent GameplayMessageEvent;
	
private:

	void OnAbilityOnCooldown();
	void OnOutOfMana();
	
	UPROPERTY()
	AHPlayerCharacter* Character;

	static FText GetGameplayEventMessage(const FString& Key);
	
};
