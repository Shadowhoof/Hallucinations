// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"

#include "Core/HInteractable.h"
#include "GameFramework/Actor.h"
#include "HLevelExit.generated.h"

UCLASS()
class HALLUCINATIONS_API AHLevelExit : public AActor, public IHInteractable
{
	GENERATED_BODY()
	
public:	

	AHLevelExit();

	virtual void InteractWith(AHCharacter* Interactor) override;

	virtual FVector GetInteractableLocation() const override;
	
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Level")
	FName LevelName;

	// Tag of a PlayerStart actor which will be used as a spawn point on the next level
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Level")
	FName PlayerStartTag;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;
	
};
