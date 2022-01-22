// Created by Dmitrii Korovin


#include "Level/HFadeTrigger.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Level/HFadeableComponent.h"
#include "Utils/HLogUtils.h"

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
		return !Actor || !Actor->GetComponentByClass(UHFadeableComponent::StaticClass());
	});
	
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AHFadeTrigger::OnBeginOverlap);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AHFadeTrigger::OnEndOverlap);

	// overlap detection at begin play seems to depend on initialization order so we'll delay the check until next tick
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AHFadeTrigger::CheckForInitialOverlap);
}

void AHFadeTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ACharacter>(OtherActor) != GetPlayerCharacter())
	{
		return;
	}

	FadeOutActors();
}

void AHFadeTrigger::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<ACharacter>(OtherActor) != GetPlayerCharacter())
	{
		return;
	}
	
	FadeInActors();
}

void AHFadeTrigger::FadeOutActors()
{
	for (auto& Actor : FadeableActors)
	{
		UHFadeableComponent* FadeableComponent = Cast<UHFadeableComponent>(Actor->GetComponentByClass(UHFadeableComponent::StaticClass()));
		FadeableComponent->FadeOut(this);
	}
}

void AHFadeTrigger::FadeInActors()
{
	for (auto& Actor : FadeableActors)
	{
		UHFadeableComponent* FadeableComponent = Cast<UHFadeableComponent>(Actor->GetComponentByClass(UHFadeableComponent::StaticClass()));
		FadeableComponent->FadeIn(this);
	}
}

ACharacter* AHFadeTrigger::GetPlayerCharacter() const
{
	return UGameplayStatics::GetPlayerCharacter(this, 0);
}

void AHFadeTrigger::CheckForInitialOverlap()
{
	if (BoxComponent->IsOverlappingActor(GetPlayerCharacter()))
	{
		FadeOutActors();
	}
}	

