// Created by Dmitrii Korovin


#include "Level/HFadeableMesh.h"

#include "HConstants.h"

AHFadeableMesh::AHFadeableMesh()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
}

void AHFadeableMesh::FadeOut()
{
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Click, ECR_Ignore);
	StaticMeshComponent->SetHiddenInGame(true);
}

void AHFadeableMesh::FadeIn()
{
	StaticMeshComponent->SetCollisionResponseToChannel(ECC_Click, DefaultClickResponse);
	StaticMeshComponent->SetHiddenInGame(false);
}

void AHFadeableMesh::BeginPlay()
{
	Super::BeginPlay();

	DefaultClickResponse = StaticMeshComponent->GetCollisionResponseToChannel(ECC_Click);
}

