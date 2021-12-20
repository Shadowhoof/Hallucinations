// Created by Dmitrii Korovin


#include "Level/HFadeableComponent.h"

#include "HConstants.h"

DEFINE_LOG_CATEGORY(LogFade);

namespace FadeConstants
{
	// time it takes for the object to change its transparency to MinOpacity
	const float FadeTime = 0.2f;
	// default opacity of fadeable object
	const float DefaultOpacity = 1.f;
	// minimum opacity of a fadeable object, value should be between in [0.0, 1.0] range 
	const float MinOpacity = 0.f;
	// dynamic material opacity parameter name
	const FName OpacityParamName = FName("Opacity");
}


UHFadeableComponent::UHFadeableComponent() : CurrentOpacity(FadeConstants::DefaultOpacity), DesiredOpacity(FadeConstants::DefaultOpacity)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHFadeableComponent::BeginPlay()
{
	Super::BeginPlay();
	
	UMeshComponent* FadeableMesh = GetFadeableMesh();
	bool bAreTransparentMaterialsValid = GetOwner()->IsHidden() || !TransparentMaterials.IsEmpty(); 
	if (!FadeableMesh || !bAreTransparentMaterialsValid)
	{
		UE_LOG(LogLevel, Warning, TEXT("FadeableComponent: Actor %s has no mesh component or its fade materials aren't set properly"), *GetOwner()->GetName())
		bIsFadeable = false;
		return;
	}

	MeshComponent = FadeableMesh;

	int32 TransparentMaterialCount = TransparentMaterials.Num();
	OpaqueMaterials.Reserve(TransparentMaterialCount);
	for (int32 Index = 0; Index < TransparentMaterialCount; ++Index)
	{
		DynamicTransparentMaterials.Add(UMaterialInstanceDynamic::Create(TransparentMaterials[Index], this));
		OpaqueMaterials.Add(FadeableMesh->GetMaterial(Index));
	}
	DefaultClickResponse = MeshComponent->GetCollisionResponseToChannel(ECC_Click);
}

UMeshComponent* UHFadeableComponent::GetFadeableMesh()
{
	return Cast<UMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
}

void UHFadeableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentOpacity == DesiredOpacity)
	{
		return;
	}

	const float PreviousOpacity = CurrentOpacity;
	if constexpr (FadeConstants::FadeTime > 0.f)
	{
		float Sign = FMath::Sign(DesiredOpacity - CurrentOpacity);
		float DeltaOpacity = Sign * DeltaTime / FadeConstants::FadeTime;
		CurrentOpacity = FMath::Clamp(CurrentOpacity + DeltaOpacity, FadeConstants::MinOpacity, FadeConstants::DefaultOpacity);
	}
	else
	{
		CurrentOpacity = DesiredOpacity;
	}

	if constexpr (FadeConstants::MinOpacity == 0.f)
	{
		// If actor's opacity is reduced to 0.0 then it's beneficial to hide it completely so that those objects don't
		// affect rendering. Multiple transparent objects in one line dramatically increase the shader complexity.  
		if (PreviousOpacity > 0.f && CurrentOpacity == 0.f)
		{
			GetOwner()->SetActorHiddenInGame(true);
		}
		else if (PreviousOpacity == 0.f && CurrentOpacity > 0.f)
		{
			GetOwner()->SetActorHiddenInGame(false);
		}
	}
	
	bool bReturnedToDefault = CurrentOpacity == FadeConstants::DefaultOpacity && DesiredOpacity == FadeConstants::DefaultOpacity;
	for (uint8 Index = 0; Index < DynamicTransparentMaterials.Num(); ++Index)
	{
		UMaterialInstanceDynamic* Material = DynamicTransparentMaterials[Index];
		Material->SetScalarParameterValue(FadeConstants::OpacityParamName, CurrentOpacity);

		if (bReturnedToDefault)
		{
			MeshComponent->SetMaterial(Index, OpaqueMaterials[Index]);
		}
	}
}

void UHFadeableComponent::FadeOut(AActor* FadeSource)
{
	if (!bIsFadeable)
	{
		return;
	}

	if (!FadeSource)
	{
		UE_LOG(LogFade, Error, TEXT("FadeOut: provided FadeSource for %s is nullptr"), *GetOwner()->GetName());
		return;
	}

	if (FadeSources.IsEmpty())
	{
		DesiredOpacity = FadeConstants::MinOpacity;
		MeshComponent->SetCollisionResponseToChannel(ECC_Click, ECR_Ignore);
		for (uint8 Index = 0; Index < DynamicTransparentMaterials.Num(); ++Index)
		{
			MeshComponent->SetMaterial(Index, DynamicTransparentMaterials[Index]);
		}
		UE_LOG(LogFade, Verbose, TEXT("Fading out actor %s with source %s"), *GetOwner()->GetName(), *FadeSource->GetName());
	}
	else
	{
		UE_LOG(LogFade, Verbose, TEXT("Actor %s is already faded out, adding new source: %s"), *GetOwner()->GetName(), *FadeSource->GetName());
	}
	FadeSources.Add(FadeSource);
}

void UHFadeableComponent::FadeIn(AActor* FadeSource)
{
	if (!bIsFadeable || !FadeSource)
	{
		return;
	}

	if (int32 ElementCount = FadeSources.Remove(FadeSource); ElementCount == 0)
	{
		UE_LOG(LogFade, Error, TEXT("FadeIn: provided FadeSource %s for actor %s is not in FadeSources array"), *FadeSource->GetName(), *GetOwner()->GetName());
		return;
	}

	if (FadeSources.IsEmpty())
	{
		DesiredOpacity = FadeConstants::DefaultOpacity;
		MeshComponent->SetCollisionResponseToChannel(ECC_Click, DefaultClickResponse);

		UE_LOG(LogFade, Verbose, TEXT("Fading in actor %s after removing source %s"), *GetOwner()->GetName(), *FadeSource->GetName());
	}
	else
	{
		UE_LOG(LogFade, Verbose, TEXT("Actor %s is still faded out, removing source: %s"), *GetOwner()->GetName(), *FadeSource->GetName());
	}
}

