// Created by Dmitrii Korovin


#include "Level/HFadeTrigger.h"
#include "Components/BoxComponent.h"
#include "Level/HFadeable.h"

AHFadeTrigger::AHFadeTrigger()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	RootComponent = BoxComponent;
}

void AHFadeTrigger::BeginPlay()
{
	Super::BeginPlay();

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AHFadeTrigger::OnBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AHFadeTrigger::OnEndOverlap);
}

void AHFadeTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (auto& Object : FadeableObjects)
	{
		IHFadeable* Fadeable = Cast<IHFadeable>(Object);
		if (Fadeable)
		{
			Fadeable->FadeOut();
		}
	}
}

void AHFadeTrigger::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for (auto& Object : FadeableObjects)
	{
		IHFadeable* Fadeable = Cast<IHFadeable>(Object);
		if (Fadeable)
		{
			Fadeable->FadeIn();
		}
	}
}

