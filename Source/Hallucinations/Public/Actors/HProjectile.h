// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HProjectile.generated.h"

class UProjectileMovementComponent;


// Holds projectile data filled by the shooter
struct FHProjectileData
{
	float Damage;
	TSubclassOf<UDamageType> DamageType;
};


UCLASS()
class HALLUCINATIONS_API AHProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHProjectile();

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement")
	float MaxRange;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float DistanceTraveled;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	FHProjectileData Data;

	UStaticMeshComponent* GetMesh() const;
};
