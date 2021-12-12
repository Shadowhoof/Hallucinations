// Created by Dmitrii Korovin

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HFadeTrigger.generated.h"

class UBoxComponent;

UCLASS()
class HALLUCINATIONS_API AHFadeTrigger : public AActor
{
	GENERATED_BODY()
	
public:	

	AHFadeTrigger();

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fade")
	UBoxComponent* BoxComponent;
	
	// Objects which will fade when player character is inside this trigger
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Fade")
	TArray<AActor*> FadeableActors;

private:

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
};
