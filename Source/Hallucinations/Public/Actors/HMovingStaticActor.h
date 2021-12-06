// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HMovingStaticActor.generated.h"


class UArrowComponent;


USTRUCT(BlueprintType)
struct FStaticMovementParams
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category = "Movement", Meta = (ClampMin = 0.f))
	float Speed = 0.f;

	UPROPERTY(EditAnywhere, Category = "Movement", Meta = (ClampMin = 0.f))
	float PostDelay = 0.f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bIsAutomatic = false;
};


UCLASS()
class HALLUCINATIONS_API AHMovingStaticActor : public AActor
{
	GENERATED_BODY()
	
public:	

	AHMovingStaticActor();

	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UArrowComponent* ArrowComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	FVector ForwardDirection = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", Meta = (ClampMin = 0.f))
	float MovementDistance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	FStaticMovementParams ForwardParams;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
	FStaticMovementParams BackwardParams;

	virtual void MoveForward();
	virtual void MoveBackward();
	
private:
	
	enum class EMovementState
	{
		AtStartPoint,
		Forward,
		AtEndPoint,
		Backward,
	};
	
	FTimerHandle DelayTimerHandle;

	EMovementState CurrentState;
	
	void UpdateMovement(float DeltaSeconds);
	void UpdateCurrentState(float DeltaSeconds);

	void SetMovementState(EMovementState NewState);
	void TryAutoSetNextState(EMovementState NewState);

	FVector StartPosition;
	FVector EndPosition;
	float ForwardTime;
	float BackwardTime;
	
	float RemainingTime = 0.f;
	float InitialTime = 0.f;
	bool bIsStopped = false;

	bool CanMove() const;
};
