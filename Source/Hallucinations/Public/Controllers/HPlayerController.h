// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "HPlayerController.generated.h"

class UMessageHandlerComponent;
class AHPlayerCharacter;
class UHPlayerCharacterSave;
class UCameraComponent;
class AHCharacter;

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API AHPlayerController : public APlayerController, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:

	AHPlayerController();

	virtual void PlayerTick(float DeltaSeconds) override;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Target")
	FHitResult MouseoverData;

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnUnPossess() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnPlayerRestart();

	virtual FGenericTeamId GetGenericTeamId() const override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnCharacterBeginPlay(AHPlayerCharacter* PlayerCharacter);
	
protected:
	
	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnPawnDeath(AHCharacter* Killer, AActor* Victim);

	AHPlayerCharacter* GetPlayerCharacter() const;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMessageHandlerComponent* MessageHandlerComponent;
	
private:
	
	UPROPERTY()
	AActor* HoveredActor;

	void ProcessMouseover();
	void HandleHoveredActor(AActor* Actor);

	FGenericTeamId GenericTeamId;
	
};
