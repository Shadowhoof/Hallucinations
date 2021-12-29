// Created by Dmitrii Korovin


#include "Level/HLevelExit.h"

#include "HConstants.h"
#include "Kismet/GameplayStatics.h"

AHLevelExit::AHLevelExit()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = StaticMesh;
}

void AHLevelExit::BeginPlay()
{
	Super::BeginPlay();
}

void AHLevelExit::InteractWith(AHCharacter* Interactor)
{
	if (LevelName.IsNone())
	{
		return;
	}

	const FString OptionsString = HallucinationsConstants::PlayerStartTag + "=" + PlayerStartTag.ToString();
	UGameplayStatics::OpenLevel(this, LevelName, true, OptionsString);
}

FVector AHLevelExit::GetInteractableLocation() const
{
	return GetActorLocation();
}
