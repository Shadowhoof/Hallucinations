// Created by Dmitrii Korovin


#include "Level/HFadeableMesh.h"


AHFadeableMesh::AHFadeableMesh()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;
	
	FadeableComponent = CreateDefaultSubobject<UHFadeableComponent>(TEXT("FadeableComponent"));
}
