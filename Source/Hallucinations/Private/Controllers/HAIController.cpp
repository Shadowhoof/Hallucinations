// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/HAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BTNode.h"
#include "Characters/HCharacter.h"
#include "Components/HHealthComponent.h"
#include "Core/HLogCategories.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISense_Team.h"
#include "Utils/HLogUtils.h"

DEFINE_LOG_CATEGORY(LogAI);


namespace AIConstants
{
	const FName TargetActorKeyName("TargetActor");

	constexpr float CallForHelpRange = 1000.f;
}


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

	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(AIConstants::TargetActorKeyName));
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

AActor* AHAIController::GetTargetActor() const
{
	return TargetActor;
}

void AHAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AHAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	const AHCharacter* AsCharacter = Cast<AHCharacter>(InPawn);
	if (!AsCharacter)
	{
		return;
	}
	
	const uint8 TeamId = static_cast<uint8>(AsCharacter->GetHealthComponent()->GetTeam());
	SetGenericTeamId(FGenericTeamId(TeamId));
	
	AsCharacter->GetHealthComponent()->OnDamageTaken.AddDynamic(this, &AHAIController::OnPossessedPawnDamageTaken);

}

void AHAIController::OnUnPossess()
{
	if (const AHCharacter* AsCharacter = Cast<AHCharacter>(GetPawn()))
	{
		AsCharacter->GetHealthComponent()->OnDamageTaken.RemoveDynamic(this, &AHAIController::OnPossessedPawnDamageTaken);
	}
	
	Super::OnUnPossess();
}

void AHAIController::OnPossessedPawnDamageTaken(AActor* Victim, AActor* Source, float Damage)
{
	if (!Source)
	{
		return;
	}

	const FAITeamStimulusEvent Event(Victim, Source, Source->GetActorLocation(), AIConstants::CallForHelpRange);
	UAIPerceptionSystem::OnEvent(GetWorld(), Event);
}
