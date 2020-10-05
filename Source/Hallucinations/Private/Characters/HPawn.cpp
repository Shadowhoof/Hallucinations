// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HPawn.h"
#include "Components/CapsuleComponent.h"
#include "Controllers/HAIController.h"

// Sets default values
AHPawn::AHPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

