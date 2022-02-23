#pragma once

#include "Engine/DataTable.h"
#include "Progression/HAttributeType.h"
#include "HCharacterMetaData.generated.h"

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Warrior,
	Rogue,
	Mage
};


USTRUCT()
struct FCharacterClassMetaData : public FTableRowBase
{
	GENERATED_BODY()

	/** Character class for which metadata is specified */
	UPROPERTY(EditAnywhere)
	ECharacterClass Class;

	/** Identifiers of abilities that are available to class */
	UPROPERTY(EditAnywhere)
	TArray<int32> AbilityIds;

	/** Default attribute values for class */
	UPROPERTY(EditAnywhere)
	TMap<EAttributeType, float> Attributes;

	/** Character skeletal mesh for class */
	UPROPERTY(EditAnywhere)
	USkeletalMesh* SkeletalMesh;

	/** Animation blueprint for class */
	// TODO: should probably be a separate map for mesh to anim blueprint mapping? 
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAnimInstance> AnimBlueprintClass;
};