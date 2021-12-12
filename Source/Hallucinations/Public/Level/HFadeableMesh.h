// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"

#include "HFadeableComponent.h"
#include "GameFramework/Actor.h"
#include "HFadeableMesh.generated.h"

UCLASS()
class HALLUCINATIONS_API AHFadeableMesh : public AActor
{
	GENERATED_BODY()
	
public:	

	AHFadeableMesh();
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UHFadeableComponent* FadeableComponent;

};
