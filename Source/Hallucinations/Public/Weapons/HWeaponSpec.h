// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HWeaponSpec.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	Bow,
	Melee
};

/**
 * A weapon specification that contains common data which can be used by
 * multiple weapon instances of the same type.
 */
UCLASS(BlueprintType, Blueprintable)
class HALLUCINATIONS_API UHWeaponSpec : public UObject {

	GENERATED_BODY()

public:

	UHWeaponSpec();

private:

	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	UStaticMesh* StaticMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, Category = "Visual")
	FName AttachmentSocketName;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float WeaponRange = 150.f;
};
