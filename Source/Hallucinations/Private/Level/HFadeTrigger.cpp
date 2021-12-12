// Created by Dmitrii Korovin


#include "Level/HFadeTrigger.h"
#include "Components/BoxComponent.h"
#include "Level/HFadeableComponent.h"

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
	
	FadeableActors.RemoveAll([](const AActor* Actor)
	{
		return !Actor->GetComponentByClass(UHFadeableComponent::StaticClass());
	});
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AHFadeTrigger::OnBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AHFadeTrigger::OnEndOverlap);
}

void AHFadeTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	for (auto& Actor : FadeableActors)
	{
		UHFadeableComponent* FadeableComponent = Cast<UHFadeableComponent>(Actor->GetComponentByClass(UHFadeableComponent::StaticClass()));
		FadeableComponent->FadeOut();
	}
}

void AHFadeTrigger::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	for (auto& Actor : FadeableActors)
	{
		UHFadeableComponent* FadeableComponent = Cast<UHFadeableComponent>(Actor->GetComponentByClass(UHFadeableComponent::StaticClass()));
		FadeableComponent->FadeIn();
	}
}

