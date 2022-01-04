// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/HPlayerCharacter.h"

#include "HConstants.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "NavigationSystem.h"
#include "Engine/World.h"
#include "NavigationPath.h"
#include "Abilities/HAbilityComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Controllers/HPlayerController.h"
#include "Components/HAttackComponent.h"
#include "Components/HFollowComponent.h"
#include "Components/HHealthComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Inventory/HInventoryComponent.h"
#include "Inventory/HEquipmentComponent.h"
#include "Progression/HAttributeComponent.h"
#include "Core/HInteractable.h"
#include "Core/Subsystems/Save/HSaveSubsystem.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Level/HFadeableComponent.h"
#include "Utils/HLogUtils.h"

namespace Constants
{
	const FRotator CameraRotation{-45.f, 45.f, 0.f};
	const float MinimapSceneComponentHeight = 10000.f;
	const float MinimapCaptureWidth = 2048.f;

	const float FadeSphereTraceRadius = 200.f;
	const float FadeInDelay = 0.5f;
}


AHPlayerCharacter::AHPlayerCharacter()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 1500.f;
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->SetWorldRotation(Constants::CameraRotation);

	MinCameraDistance = 300.f;
	MaxCameraDistance = 2000.f;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	AttributeComponent = CreateDefaultSubobject<UHAttributeComponent>(TEXT("AttributeComponent"));
	ActionBarComponent = CreateDefaultSubobject<UHActionBarComponent>(TEXT("ActionBarComponent"));
	InventoryComponent = CreateDefaultSubobject<UHInventoryComponent>(TEXT("InventoryComponent"));
	EquipmentComponent = CreateDefaultSubobject<UHEquipmentComponent>(TEXT("EquipmentComponent"));

	MinimapCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("MinimapCaptureComponent"));
	MinimapCaptureComponent->SetupAttachment(RootComponent);
	MinimapCaptureComponent->ProjectionType = ECameraProjectionMode::Orthographic;
	MinimapCaptureComponent->SetUsingAbsoluteRotation(true);
	MinimapCaptureComponent->SetWorldRotation(FRotator(-90.f, 0.f, 0.f));
	// disable skeletal meshes on minimap
	MinimapCaptureComponent->ShowFlags.SetSkeletalMeshes(false);
	
	GetCharacterMovement()->bOrientRotationToMovement = true;

	bIsHoldingPrimaryAction = false;
}

// Called when the game starts or when spawned
void AHPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	MinimapCaptureComponent->SetRelativeLocation(FVector(0.f, 0.f, Constants::MinimapSceneComponentHeight));
	MinimapCaptureComponent->OrthoWidth = Constants::MinimapCaptureWidth;

	if (IsLocallyControlled())
	{
		// disable all click collision for local player
		BoxComponent->SetCollisionResponseToChannel(ECC_Click, ECR_Ignore);
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Click, ECR_Ignore);
		GetMesh()->SetCollisionResponseToChannel(ECC_Click, ECR_Ignore);

		// enable terrain fade triggers
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_FadeTrigger, ECR_Overlap);
	}
}

// Called every frame
void AHPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bIsHoldingPrimaryAction)
	{
		PrimaryAction(true);
	}

	UpdateFadeableObstructions(DeltaTime);
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
	PlayerInputComponent->BindAction<FUseAbilityDelegate>("Ability1", IE_Pressed, this, &AHPlayerCharacter::UseAbility, 0);
	PlayerInputComponent->BindAction<FUseAbilityDelegate>("Ability2", IE_Pressed, this, &AHPlayerCharacter::UseAbility, 1);
	PlayerInputComponent->BindAction<FUseAbilityDelegate>("Ability3", IE_Pressed, this, &AHPlayerCharacter::UseAbility, 2);
	PlayerInputComponent->BindAction<FUseAbilityDelegate>("Ability4", IE_Pressed, this, &AHPlayerCharacter::UseAbility, 3);
	PlayerInputComponent->BindAction<FUseAbilityDelegate>("Ability5", IE_Pressed, this, &AHPlayerCharacter::UseAbility, 4);
	PlayerInputComponent->BindAction<FUseAbilityDelegate>("Ability6", IE_Pressed, this, &AHPlayerCharacter::UseAbility, 5);
	PlayerInputComponent->BindAction<FUseAbilityDelegate>("Ability7", IE_Pressed, this, &AHPlayerCharacter::UseAbility, 6);
	PlayerInputComponent->BindAction<FUseAbilityDelegate>("Ability8", IE_Pressed, this, &AHPlayerCharacter::UseAbility, 7);
	PlayerInputComponent->BindAction<FUseAbilityDelegate>("Ability9", IE_Pressed, this, &AHPlayerCharacter::UseAbility, 8);

	PlayerInputComponent->BindAction("SkillBook", IE_Pressed, this, &AHPlayerCharacter::ToggleSkillBook);
	PlayerInputComponent->BindAction("Inventory", IE_Pressed, this, &AHPlayerCharacter::ToggleInventory);
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

void AHPlayerCharacter::UseAbility(int32 Index)
{
	if (!IsBusy())
	{
		ActionBarComponent->UseAbilityByIndex(Index);
	}
}

UHPlayerCharacterSave* AHPlayerCharacter::GetSaveData()
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	UHSaveSubsystem* SaveSubsystem = GameInstance->GetSubsystem<UHSaveSubsystem>();
	return SaveSubsystem->GetCharacterSaveData();
}

FPlayerCharacterSessionState AHPlayerCharacter::GetSessionState()
{
	FPlayerCharacterSessionState State;
	State.Health = HealthComponent->GetHealthPercentage();
	AbilityComponent->GetPersistentState(State.AbilityCooldowns);
	return State;
}

void AHPlayerCharacter::RestoreSessionState(const FPlayerCharacterSessionState& State)
{
	HealthComponent->RestorePersistentState(State.Health);
	AbilityComponent->RestorePersistentState(State.AbilityCooldowns);
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

void AHPlayerCharacter::PrimaryAction(bool bIsRepeated)
{
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

		if (IHInteractable* Interactable = Cast<IHInteractable>(MouseoverActor); Interactable)
		{
			
		}
	}
}

void AHPlayerCharacter::UpdateFadeableObstructions(float DeltaTime)
{
	TArray<FHitResult> TraceResult;
	FCollisionObjectQueryParams ObjectQueryParams{ECC_WorldStatic};
	GetWorld()->LineTraceMultiByObjectType(TraceResult, CameraComponent->GetComponentLocation(), GetActorLocation(), ObjectQueryParams);

	for (auto& Entry : FadedActorMap)
	{
		Entry.Value -= DeltaTime;
	}
	
	for (auto& Hit : TraceResult)
	{
		AActor* HitActor = Hit.GetActor();
		UHFadeableComponent* FadeableComponent = Cast<UHFadeableComponent>(HitActor->GetComponentByClass(UHFadeableComponent::StaticClass()));
		if (!FadeableComponent)
		{
			continue;
		}

		if (!FadedActorMap.Contains(HitActor))
		{
			FadeableComponent->FadeOut(this);
			FadedActorMap.Add(HitActor, Constants::FadeInDelay);
		}
		else
		{
			FadedActorMap[HitActor] = Constants::FadeInDelay;
		}
	}

	for (auto It = FadedActorMap.CreateIterator(); It; ++It)
	{
		if (It.Value() <= 0.f)
		{
			UHFadeableComponent* FadeableComponent = Cast<UHFadeableComponent>(It.Key()->GetComponentByClass(UHFadeableComponent::StaticClass()));
			FadeableComponent->FadeIn(this);
			It.RemoveCurrent();
		}
	}
}

