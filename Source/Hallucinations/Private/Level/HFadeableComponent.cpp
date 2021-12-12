// Created by Dmitrii Korovin


#include "Level/HFadeableComponent.h"

#include "HConstants.h"

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
	if (!FadeableMesh || !TransparentMaterial)
	{
		UE_LOG(LogLevel, Warning, TEXT("FadeableComponent: Actor %s has no mesh component or its fade material isn't set properly"), *GetOwner()->GetName())
		bIsFadeable = false;
		return;
	}

	OpaqueMaterial = FadeableMesh->GetMaterial(0);
	MeshComponent = FadeableMesh;
	DynamicTransparentMaterial = UMaterialInstanceDynamic::Create(TransparentMaterial, this);
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

	DynamicTransparentMaterial->SetScalarParameterValue(FadeConstants::OpacityParamName, CurrentOpacity);
	if (CurrentOpacity == FadeConstants::DefaultOpacity && DesiredOpacity == FadeConstants::DefaultOpacity)
	{
		MeshComponent->SetMaterial(0, OpaqueMaterial);
	}
}

void UHFadeableComponent::FadeOut()
{
	if (!bIsFadeable)
	{
		return;
	}
	
	DesiredOpacity = FadeConstants::MinOpacity;
	MeshComponent->SetCollisionResponseToChannel(ECC_Click, ECR_Ignore);
	MeshComponent->SetMaterial(0, DynamicTransparentMaterial);
}

void UHFadeableComponent::FadeIn()
{
	if (!bIsFadeable)
	{
		return;
	}
	
	DesiredOpacity = FadeConstants::DefaultOpacity;
	MeshComponent->SetCollisionResponseToChannel(ECC_Click, DefaultClickResponse);
}

