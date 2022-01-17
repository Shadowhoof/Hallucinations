// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HWeapon.generated.h"

struct FAttackResult;


USTRUCT(BlueprintType)
struct FWeaponAttackParameters
{
	GENERATED_BODY()

	/** Animation montage that will be used during attack */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* Animation;

	/** Time between attack animation starting and attack landing */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackPoint;

	/**
	 *	Time between attack landing and attack animation finishing. Attack backswing can be cancelled by giving
	 *	another action to a character
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackBackswing;

	/** Time before a new attack can begin since the start of the previous attack */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackSpeed;

	/** Reach of a weapon. Targets must be within this range for an attack to start */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackRange;
};


UCLASS()
class HALLUCINATIONS_API AHWeapon : public AActor
{
	GENERATED_BODY()
	
public:
	
	AHWeapon();

	FName GetAttachmentSocketName() const;

	// Is weapon able to reach the target
	bool IsInRange(AActor* OwnerActor, AActor* TargetActor) const;

	const FWeaponAttackParameters& GetAttackParameters();

	virtual void AttackActor(AActor* TargetActor, bool bIsAbilityAttack, FAttackResult& OutResult) PURE_VIRTUAL(AHWeapon::AttackActor);
	
	virtual void AttackLocation(const FVector& TargetLocation, bool bIsAbilityAttack, FAttackResult& OutResult) PURE_VIRTUAL(AHWeapon::AttackLocation);
	
protected:

	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attachment")
	FName AttachmentSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	FWeaponAttackParameters AttackParameters;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack")
	TSubclassOf<UDamageType> DamageType;

};
