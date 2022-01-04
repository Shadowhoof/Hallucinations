// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HCharacter.h"
#include "Core/Subsystems/Save/HCharacterSave.h"
#include "HPlayerCharacter.generated.h"

class UHEquipmentComponent;
class UHInventoryComponent;
class UHPlayerCharacterSave;
class UCameraComponent;
class USpringArmComponent;
class AHProjectile;
class AHPlayerController;
class UHAttributeComponent;
class UHAbilityComponent;
class UHActionBarComponent;
class USceneCaptureComponent2D;

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API AHPlayerCharacter : public AHCharacter
{
	GENERATED_BODY()

public:

	AHPlayerCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual AActor* GetTargetActor() const override;

	virtual FVector GetTargetLocation() const override;

	void UseAbility(int32 Index);

	UHPlayerCharacterSave* GetSaveData();

	FPlayerCharacterSessionState GetSessionState();
	void RestoreSessionState(const FPlayerCharacterSessionState& State);
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHAttributeComponent* AttributeComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHActionBarComponent* ActionBarComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHEquipmentComponent* EquipmentComponent;

	// camera
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MinCameraDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MaxCameraDistance;

	UPROPERTY(EditAnywhere, Category = "Minimap")
	USceneCaptureComponent2D* MinimapCaptureComponent;

	bool bIsHoldingPrimaryAction;
	
	virtual void BeginPlay() override;

	void Move(float Value);
	void Strafe(float Value);
	void Zoom(float Value);

	void OnPrimaryActionPress();
	void OnPrimaryActionRelease();
	void PrimaryAction(bool bIsRepeated);

	DECLARE_DELEGATE_OneParam(FUseAbilityDelegate, int32)

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void ToggleSkillBook();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void ToggleInventory();

private:

	void UpdateFadeableObstructions(float DeltaTime);

	UPROPERTY()
	TMap<AActor*, float> FadedActorMap;
	
};
