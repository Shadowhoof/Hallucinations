// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "NavigationSystem.h"
#include "Engine/World.h"
#include "NavigationPath.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AHPlayerCharacter::AHPlayerCharacter() {

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 1500.f;
	SpringArmComponent->bDoCollisionTest = false;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bIsHoldingPrimaryAction = false;
}

// Called when the game starts or when spawned
void AHPlayerCharacter::BeginPlay() {
	Super::BeginPlay();

	SpringArmComponent->SetUsingAbsoluteRotation(true);
}

// Called every frame
void AHPlayerCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (bIsHoldingPrimaryAction) {
		PrimaryAction();
	}
}

// Called to bind functionality to input
void AHPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move", this, &AHPlayerCharacter::Move);
	PlayerInputComponent->BindAxis("Strafe", this, &AHPlayerCharacter::Strafe);

	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &AHPlayerCharacter::OnPrimaryActionPress);
	PlayerInputComponent->BindAction("PrimaryAction", IE_Released, this, &AHPlayerCharacter::OnPrimaryActionRelease);
}

void AHPlayerCharacter::Move(float Value) {
	AddMovementInput(GetActorForwardVector() * Value);
}

void AHPlayerCharacter::Strafe(float Value) {
	AddMovementInput(GetActorRightVector() * Value);
}

void AHPlayerCharacter::OnPrimaryActionPress() {
	bIsHoldingPrimaryAction = true;
}

void AHPlayerCharacter::OnPrimaryActionRelease() {
	bIsHoldingPrimaryAction = false;
}

void AHPlayerCharacter::PrimaryAction() {
	APlayerController* controller = Cast<APlayerController>(GetController());
	if (!controller) {
		UE_LOG(LogTemp, Log, TEXT("No player controller assigned"));
		return;
	}

	float screenSpaceX, screenSpaceY;
	controller->GetMousePosition(screenSpaceX, screenSpaceY);
	FHitResult hitResult;
	controller->GetHitResultAtScreenPosition(FVector2D(screenSpaceX, screenSpaceY), controller->CurrentClickTraceChannel, FCollisionQueryParams(), hitResult);

	if (hitResult.bBlockingHit) {
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(controller, hitResult.ImpactPoint);
	}
}

