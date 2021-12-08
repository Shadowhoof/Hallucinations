// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"

#include "HFadeable.h"
#include "GameFramework/Actor.h"
#include "HFadeableMesh.generated.h"

UCLASS()
class HALLUCINATIONS_API AHFadeableMesh : public AActor, public IHFadeable
{
	GENERATED_BODY()
	
public:	

	AHFadeableMesh();

	virtual void FadeOut() override;
	virtual void FadeIn() override;
	
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

private:

	ECollisionResponse DefaultClickResponse;

};
