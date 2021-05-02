// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HPlayerCharacter.h"

#include "HConstants.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "NavigationSystem.h"
#include "Engine/World.h"
#include "NavigationPath.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controllers/HPlayerController.h"
#include "Actors/HProjectile.h"
#include "Utils/HUtils.h"
#include "Components/HAttackComponent.h"
#include "Components/HFollowComponent.h"
#include "Components/HAttributeComponent.h"
#include "Components/HHealthComponent.h"
#include "Abilities/HAbilityComponent.h"

// Sets default values
AHPlayerCharacter::AHPlayerCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 1500.f;
	SpringArmComponent->bDoCollisionTest = false;

	MinCameraDistance = 300.f;
	MaxCameraDistance = 2000.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	AttributeComponent = CreateDefaultSubobject<UHAttributeComponent>(TEXT("AttributeComponent"));
	AbilityComponent = CreateDefaultSubobject<UHAbilityComponent>(TEXT("AbilityComponent"));
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bIsHoldingPrimaryAction = false;
}

// Called when the game starts or when spawned
void AHPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SpringArmComponent->SetUsingAbsoluteRotation(true);
}

// Called every frame
void AHPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsHoldingPrimaryAction)
		PrimaryAction(true);
}

// Called to bind functionality to input
void AHPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Move", this, &AHPlayerCharacter::Move);
	PlayerInputComponent->BindAxis("Strafe", this, &AHPlayerCharacter::Strafe);
	PlayerInputComponent->BindAxis("Zoom", this, &AHPlayerCharacter::Zoom);

	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &AHPlayerCharacter::OnPrimaryActionPress);
	PlayerInputComponent->BindAction("PrimaryAction", IE_Released, this, &AHPlayerCharacter::OnPrimaryActionRelease);

	// Abilities
	PlayerInputComponent->BindAction<FUseAbilityDelegate>("Ability1", IE_Pressed, this, &AHPlayerCharacter::UseAbility, (uint8)0);
	PlayerInputComponent->BindAction<FUseAbilityDelegate>("Ability2", IE_Pressed, this, &AHPlayerCharacter::UseAbility, (uint8)1);
	PlayerInputComponent->BindAction<FUseAbilityDelegate>("Ability3", IE_Pressed, this, &AHPlayerCharacter::UseAbility, (uint8)2);
	PlayerInputComponent->BindAction<FUseAbilityDelegate>("Ability4", IE_Pressed, this, &AHPlayerCharacter::UseAbility, (uint8)3);
}

AActor* AHPlayerCharacter::GetTargetActor() const
{
	AHPlayerController* PlayerController = Cast<AHPlayerController>(Controller);
	if (!PlayerController)
	{
		return nullptr;
	}

	FHitResult& MouseoverData = PlayerController->MouseoverData;
	if (MouseoverData.bBlockingHit)
	{
		AActor* MouseoverActor = MouseoverData.GetActor();
		if (HealthComponent->IsEnemy(MouseoverActor))
		{
			return MouseoverActor;
		}
	}

	return nullptr;
}

FVector AHPlayerCharacter::GetTargetLocation() const
{
	AHPlayerController* PlayerController = Cast<AHPlayerController>(Controller);
	if (!PlayerController)
	{
		return FHConstants::NullVector;
	}

	FHitResult& MouseoverData = PlayerController->MouseoverData;
	if (MouseoverData.bBlockingHit)
	{
		return MouseoverData.ImpactPoint;
	}

	return FHConstants::NullVector;
}

void AHPlayerCharacter::Move(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

void AHPlayerCharacter::Strafe(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

void AHPlayerCharacter::Zoom(float Value)
{
	SpringArmComponent->TargetArmLength = FMath::Clamp(SpringArmComponent->TargetArmLength + Value * 50.f, MinCameraDistance, MaxCameraDistance);
}

void AHPlayerCharacter::OnPrimaryActionPress()
{
	bIsHoldingPrimaryAction = true;
	PrimaryAction(false);
}

void AHPlayerCharacter::OnPrimaryActionRelease()
{
	bIsHoldingPrimaryAction = false;
	AttackComponent->CancelActorLock();
}

void AHPlayerCharacter::PrimaryAction(bool bIsRepeated) {
	AHPlayerController* PlayerController = Cast<AHPlayerController>(Controller);
	if (!PlayerController)
	{
		return;
	}

	FHitResult& MouseoverData = PlayerController->MouseoverData;
	if (MouseoverData.bBlockingHit) {
		const bool bIsGroundAttack = PlayerController->IsInputKeyDown(EKeys::LeftShift);
		AActor* MouseoverActor = MouseoverData.GetActor();
		const bool bIsActorAttackable = UHAttackComponent::CanBeAttacked(this, MouseoverActor);

		if (bIsGroundAttack || bIsActorAttackable || AttackComponent->GetCurrentAttackMode() == EAttackMode::LockedActor)
		{
			AttackComponent->HandlePlayerAttack(MouseoverData, bIsRepeated, bIsGroundAttack);
		}
		else
		{
			AttackComponent->StopAttacking();
			if (UHFollowComponent::CanBeFollowed(MouseoverActor))
			{
				FollowComponent->MoveToActor(MouseoverActor);
			}
			else
			{
				FollowComponent->MoveToLocation(MouseoverData.ImpactPoint);
			}
		}
	}
}

void AHPlayerCharacter::UseAbility(uint8 Index)
{
	AbilityComponent->UseAbility(Index);
}

