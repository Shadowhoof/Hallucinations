// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/HAIController.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BTNode.h"
#include "Characters/HCharacter.h"
#include "Components/HHealthComponent.h"
#include "Core/HLogCategories.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig.h"
#include "Perception/AISense_Sight.h"
#include "Utils/HLogUtils.h"

DEFINE_LOG_CATEGORY(LogAI);


AHAIController::AHAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

void AHAIController::GetCurrentlySightPerceivedActors(TArray<AActor*>& OutActors)
{
	PerceptionComponent->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), OutActors);
}

void AHAIController::GetKnownSightPerceivedActors(TArray<AActor*>& OutActors)
{
	PerceptionComponent->GetKnownPerceivedActors(UAISense_Sight::StaticClass(), OutActors);
}

void AHAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

ETeamAttitude::Type AHAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const IGenericTeamAgentInterface* Agent = Cast<IGenericTeamAgentInterface>(&Other);
	if (!Agent)
	{
		Agent = Cast<IGenericTeamAgentInterface>(Other.GetInstigatorController());
	}
	return Agent ? FGenericTeamId::GetAttitude(GetGenericTeamId(), Agent->GetGenericTeamId()) : ETeamAttitude::Neutral;
}

void AHAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AHAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	AHCharacter* AsCharacter = Cast<AHCharacter>(InPawn);
	if (AsCharacter)
	{
		uint8 TeamId = (uint8) AsCharacter->GetHealthComponent()->GetTeam();
		UE_LOG(LogTemp, Log, TEXT("TeamId: %d"), TeamId);
		SetGenericTeamId(FGenericTeamId(TeamId));
	}

}
