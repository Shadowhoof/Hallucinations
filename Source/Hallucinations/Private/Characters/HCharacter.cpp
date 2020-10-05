// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "NavigationSystem.h"
#include "Controllers/HAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/HHealthComponent.h"

// Sets default values
AHCharacter::AHCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthComponent = CreateDefaultSubobject<UHHealthComponent>(TEXT("HealthComponent"));
	
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;

	AIControllerClass = AHAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AHCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

