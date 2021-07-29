// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HCharacter.h"
#include "HPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class AHProjectile;
class AHPlayerController;
class UHAttributeComponent;
class UHAbilityComponent;

/**
 * 
 */
UCLASS()
class HALLUCINATIONS_API AHPlayerCharacter : public AHCharacter
{
	GENERATED_BODY()

public:

	AHPlayerCharacter();
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHAttributeComponent* AttributeComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MinCameraDistance;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float MaxCameraDistance;

	bool bIsHoldingPrimaryAction;
	
	virtual void BeginPlay() override;

	void Move(float Value);
	void Strafe(float Value);
	void Zoom(float Value);

	void OnPrimaryActionPress();
	void OnPrimaryActionRelease();
	void PrimaryAction(bool bIsRepeated);

	DECLARE_DELEGATE_OneParam(FUseAbilityDelegate, uint8)

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual AActor* GetTargetActor() const override;

	virtual FVector GetTargetLocation() const override;
};
