// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HFadeableComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HALLUCINATIONS_API UHFadeableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHFadeableComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FadeOut();
	void FadeIn();
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fade")
	UMaterialInterface* TransparentMaterial;
	
	virtual UMeshComponent* GetFadeableMesh();
	
private:

	bool bIsFadeable = true;
	
	TObjectPtr<UMeshComponent> MeshComponent;

	UPROPERTY()
	UMaterialInterface* OpaqueMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicTransparentMaterial;

	ECollisionResponse DefaultClickResponse;
	
	float CurrentOpacity;
	float DesiredOpacity;
	
};
