// Created by Dmitrii Korovin


#include "Actors/HMovingStaticActor.h"
#include "Components/ArrowComponent.h"


AHMovingStaticActor::AHMovingStaticActor()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	ArrowComponent = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
	ArrowComponent->SetupAttachment(RootComponent);
	ArrowComponent->ArrowColor = FColor::Red;
}

void AHMovingStaticActor::BeginPlay()
{
	Super::BeginPlay();

	if (!CanMove())
	{
		bIsStopped = true;
		return;
	}

	FVector WorldSpaceDirection = GetActorTransform().TransformVector(ForwardDirection.GetSafeNormal());
	UE_LOG(LogTemp, Log, TEXT("Direction before: %s, direction after: %s"), *ForwardDirection.ToString(), *WorldSpaceDirection.ToString());
	StartPosition = GetActorLocation();
	EndPosition = StartPosition + WorldSpaceDirection * MovementDistance;

	ForwardTime = ForwardParams.Speed > 0.f ? MovementDistance / ForwardParams.Speed : 0.f;
	BackwardTime = BackwardParams.Speed > 0.f ? MovementDistance / BackwardParams.Speed : 0.f;
}

void AHMovingStaticActor::MoveForward()
{
	if (!CanMove() || CurrentState != EMovementState::AtStartPoint)
	{
		return;
	}

	SetMovementState(EMovementState::Forward);
}

void AHMovingStaticActor::MoveBackward()
{
	if (!CanMove() || CurrentState != EMovementState::AtEndPoint)
	{
		return;
	}

	SetMovementState(EMovementState::Backward);
}

void AHMovingStaticActor::UpdateMovement(float DeltaSeconds)
{
	if (bIsStopped)
	{
		return;
	}
	
	UpdateCurrentState(DeltaSeconds);
	if (RemainingTime <= 0.f)
	{
		float DeltaTimeOverflow = -RemainingTime;
		switch (CurrentState)
		{
		case EMovementState::Forward:
			TryAutoSetNextState(EMovementState::AtEndPoint);
			break;
		case EMovementState::AtEndPoint:
			TryAutoSetNextState(EMovementState::Backward);
			break;
		case EMovementState::Backward:
			TryAutoSetNextState(EMovementState::AtStartPoint);
			break;
		case EMovementState::AtStartPoint:
			TryAutoSetNextState(EMovementState::Forward);
			break;
		default:
			break;
		}

		if (DeltaTimeOverflow > 0.f)
		{
			UpdateCurrentState(DeltaTimeOverflow);
		}
	}
}

void AHMovingStaticActor::UpdateCurrentState(float DeltaSeconds)
{
	RemainingTime -= DeltaSeconds;
	
	float Progress = InitialTime > 0.f ? FMath::Min(1.f - RemainingTime / InitialTime, 1.f) : 1.f;
	switch (CurrentState)
	{
	case EMovementState::Forward:
		SetActorLocation(FMath::Lerp(StartPosition, EndPosition, Progress));
		break;
	case EMovementState::Backward:
		SetActorLocation(FMath::Lerp(EndPosition, StartPosition, Progress));
		break;
	default:
		break;
	}
}

void AHMovingStaticActor::SetMovementState(EMovementState NewState)
{
	CurrentState = NewState;
	bIsStopped = false;
	float Duration = 0.f;
	switch (NewState)
	{
	case EMovementState::AtStartPoint:
		Duration = BackwardParams.PostDelay;
		break;
	case EMovementState::Forward:
		Duration = ForwardTime;
		break;
	case EMovementState::AtEndPoint:
		Duration = ForwardParams.PostDelay;
		break;
	case EMovementState::Backward:
		Duration = BackwardTime;
		break;
	default:
		break;
	}

	InitialTime = RemainingTime = FMath::Max(Duration, 0.f);
	UE_LOG(LogTemp, Log, TEXT("Movement state set to %d"), static_cast<uint8>(CurrentState));
}

void AHMovingStaticActor::TryAutoSetNextState(EMovementState NewState)
{
	if ((NewState == EMovementState::Forward && !ForwardParams.bIsAutomatic) ||
		(NewState == EMovementState::Backward && !BackwardParams.bIsAutomatic))
	{
		bIsStopped = true;
		return;
	}

	SetMovementState(NewState);
}

bool AHMovingStaticActor::CanMove() const
{
	return !ForwardDirection.IsZero() && MovementDistance > 0.f;
}

void AHMovingStaticActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateMovement(DeltaTime);
}

