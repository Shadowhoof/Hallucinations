// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HWeapon.generated.h"

UCLASS()
class HALLUCINATIONS_API AHWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHWeapon();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment")
	FName AttachmentSocketName;

	// Weapon reach distance
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float WeaponRange;

	// Animation used for attacking
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* AttackAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	float AttackSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<UDamageType> DamageType;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	FName GetAttachmentSocketName() const;

	// Is weapon able to reach the target
	bool IsInRange(AActor* OwnerActor, AActor* TargetActor) const;

	UAnimMontage* GetAttackAnimation() const;

	virtual void AttackActor(AActor* const TargetActor);

	virtual void AttackLocation(const FVector& TargetLocation);

	float GetAttackSpeed() const;

};
