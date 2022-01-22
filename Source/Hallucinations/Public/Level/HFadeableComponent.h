// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HFadeableComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogFade, Log, All);


enum class EFadeType
{
	None = 0,
	Mesh = 1 << 0,
	ClickCollision = 1 << 1
};
ENUM_CLASS_FLAGS(EFadeType)


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class HALLUCINATIONS_API UHFadeableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UHFadeableComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void FadeOut(AActor* FadeSource);
	void FadeIn(AActor* FadeSource);
	
protected:
	virtual void BeginPlay() override;

	// Transparent materials which will replace default materials on an object when it's faded. 
	// These indices should match the indices of original materials.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fade")
	TArray<TObjectPtr<UMaterialInterface>> TransparentMaterials;
	
	virtual UMeshComponent* GetFadeableMesh();
	
private:
	
	TObjectPtr<UMeshComponent> MeshComponent;

	// Actors that triggered fade on this actor. Actor will fade back in only when this array is empty. 
	UPROPERTY()
	TArray<TObjectPtr<AActor>> FadeSources;
	
	UPROPERTY()
	TArray<TObjectPtr<UMaterialInterface>> OpaqueMaterials;

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> DynamicTransparentMaterials;

	ECollisionResponse DefaultClickResponse;
	
	float CurrentOpacity;
	float DesiredOpacity;
	
	EFadeType FadeFlags;
	
};
