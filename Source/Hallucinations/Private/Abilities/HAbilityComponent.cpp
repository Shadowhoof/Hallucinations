// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HAbilityComponent.h"
#include "Abilities/HAbility.h"
#include "Abilities/HAttackAbility.h"
#include "Abilities/HSpellAbility.h"
#include "Characters/HCharacter.h"
#include "Core/HLogCategories.h"
#include "Core/Subsystems/Save/HCharacterSave.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/HLogUtils.h"
#include "Characters/HPlayerCharacter.h"
#include "Components/HFollowComponent.h"
#include "Utils/HEnumTools.h"

DEFINE_LOG_CATEGORY(LogAbility);


namespace AbilityConstants
{
	const float MinCastPoint = 0.001f;
}


const FString EmptyAbilityName = "Ability_NONE";

FString GetAbilityClassName(UHAbility* Ability)
{
	return Ability ? Ability->GetClass()->GetName() : EmptyAbilityName;
}


// Ability component

const float UHAbilityComponent::ChilledCastTimeMultiplier = 2.f;

UHAbilityComponent::UHAbilityComponent()
{
}


bool UHAbilityComponent::CanUseAbility(UHAbility* Ability, const FAbilityTargetParameters& TargetParams) const
{
	return !GetCaster()->IsBusy() && Ability->CanBeUsed(TargetParams);
}

bool UHAbilityComponent::UseSpellAbility(UHAbility* BaseAbility)
{
	ensure(BaseAbility);
	UHSpellAbility* Ability = Cast<UHSpellAbility>(BaseAbility);
	if (!Ability)
	{
		UE_LOG(LogAbility, Error, TEXT("Ability %s does not inherit UHSpellAbility"), *BaseAbility->GetSkillNameAsString());
		return false;
	}

	UHFollowComponent* FollowComponent = GetCaster()->GetFollowComponent();
	EAbilityTarget TargetType = BaseAbility->GetTargetType(CurrentTargetParams);
	switch (TargetType)
	{
	case EAbilityTarget::Actor:
		FollowComponent->RotateTowardsActor(CurrentTargetParams.Actor.Get());
		break;
	case EAbilityTarget::Point:
		FollowComponent->RotateTowardsLocation(CurrentTargetParams.Location);
		break;
	default:
		break;
	}

	const float CastPoint = FMath::Max(GetCastPoint(Ability), AbilityConstants::MinCastPoint);
	PlaySpellCastAnimation(GetCastAnimation(Ability), CastPoint);
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UHAbilityComponent::FinishCastPoint, Ability);
	GetWorld()->GetTimerManager().SetTimer(CastPointHandle, Delegate, CastPoint, false);

	QueuedSpellAbility = Ability;
	bIsCasting = true;

	return true;
}

bool UHAbilityComponent::UseAttackAbility(UHAbility* UncastAbility)
{
	ensure(UncastAbility);
	UHAttackAbility* Ability = Cast<UHAttackAbility>(UncastAbility);
	if (!Ability)
	{
		UE_LOG(LogAbility, Error, TEXT("Ability %s is not of Spell type"), *UncastAbility->GetSkillNameAsString());
		return false;
	}

	UHAttackComponent* AttackComponent = GetCaster()->GetAttackComponent();
	EAbilityTarget TargetType = UncastAbility->GetTargetType(CurrentTargetParams);
	bool bIsAttackQueued = false;
	switch (TargetType)
	{
	case EAbilityTarget::Actor:
		bIsAttackQueued = AttackComponent->AttackActorWithAbility(CurrentTargetParams.Actor.Get());
		break;
	case EAbilityTarget::Point:
		bIsAttackQueued = AttackComponent->AttackLocationWithAbility(CurrentTargetParams.Location);
		break;
	default:
		break;
	}

	if (bIsAttackQueued)
	{
		QueuedAttackAbility = Ability;
	}
	return bIsAttackQueued;
}

AActor* UHAbilityComponent::GetTargetActor() const
{
	return GetCaster()->GetTargetActor();
}

FVector UHAbilityComponent::GetTargetLocation() const
{
	return GetCaster()->GetTargetLocation();
}

AHCharacter* UHAbilityComponent::GetCaster() const
{
	return Cast<AHCharacter>(GetOwner());
}

void UHAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	Abilities.Init(nullptr, AvailableAbilities.Num());

	for (uint8 i = 0; i < AvailableAbilities.Num(); i++)
	{
		TSubclassOf<UHAbility> AbilityClass = AvailableAbilities[i];
		if (AbilityClass)
		{
			UHAbility* Ability = NewObject<UHAbility>(this, AbilityClass);
			Ability->SetAbilityComponent(this);
			Abilities[i] = Ability;
		}
	}

	UHAttackComponent* AttackComponent = GetCaster()->GetAttackComponent();
	AttackComponent->OnAttackEnded.AddDynamic(this, &UHAbilityComponent::OnAttackEnded);
}

bool UHAbilityComponent::UseAbility(UHAbility* Ability)
{
	FAbilityTargetParameters TargetParams;
	TargetParams.Actor = GetTargetActor();
	TargetParams.Location = GetTargetLocation();
	if (!CanUseAbility(Ability, TargetParams))
	{
		return false;
	}
	
	CurrentTargetParams = TargetParams;
	EAbilityType AbilityType = Ability->GetType();
	bool bIsCastingOrQueued = false;
	switch (AbilityType)
	{
	case EAbilityType::Spell:
		bIsCastingOrQueued = UseSpellAbility(Ability);
		break;
	case EAbilityType::Attack:
		bIsCastingOrQueued = UseAttackAbility(Ability);
		break;
	default:
		UE_LOG(LogAbility, Error, TEXT("Unknown ability type %s of ability %s"), *EnumAsString(AbilityType), *Ability->GetSkillNameAsString());
		break;
	}
	return bIsCastingOrQueued;
}

void UHAbilityComponent::FinishCastPoint(UHSpellAbility* Ability)
{
	Ability->OnCastFinished(CurrentTargetParams);
	QueuedSpellAbility = nullptr;
	bIsCasting = false;
	OnCastPointFinished.Broadcast(Ability);

	float CastBackswing = Ability->GetCastBackswing();
	if (CastBackswing > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(CastPointHandle, this, &UHAbilityComponent::FinishCastBackswing, CastBackswing, false);
	}
	else
	{
		FinishCastBackswing();
	}
}

void UHAbilityComponent::FinishCastBackswing()
{
	OnCastBackswingFinished.Broadcast();
}

void UHAbilityComponent::PlaySpellCastAnimation(UAnimMontage* AnimMontage, float CastPoint)
{
	float CastAnimationLength = AnimMontage->GetSectionLength(0);
	float PlayRate = CastAnimationLength / CastPoint;
	GetCaster()->PlayAnimMontage(AnimMontage, PlayRate);
}

bool UHAbilityComponent::IsCasting() const
{
	return bIsCasting;
}

void UHAbilityComponent::Interrupt()
{
	if (bIsCasting)
	{
		GetWorld()->GetTimerManager().ClearTimer(CastPointHandle);
		const UHSpellAbility* InterruptedSpell = QueuedSpellAbility;
		
		bIsCasting = false;
		QueuedSpellAbility = nullptr;
		OnAbilityCancelled.Broadcast(InterruptedSpell);
	}
}

TArray<UHAbility*> UHAbilityComponent::GetAbilities() const
{
	return Abilities;
}

bool UHAbilityComponent::HasAbility(UHAbility* Ability) const
{
	return Ability && Abilities.Contains(Ability);
}

void UHAbilityComponent::GetPersistentState(TMap<FString, float>& OutCooldownData)
{
	for (const auto& Ability : Abilities)
	{
		OutCooldownData.Add(GetAbilityClassName(Ability), Ability->GetRemainingCooldownPercentage());
	}
}

void UHAbilityComponent::RestorePersistentState(const TMap<FString, float>& CooldownData)
{
	for (const auto& Ability : Abilities)
	{
		if (FString Name = GetAbilityClassName(Ability); CooldownData.Contains(Name))
		{
			Ability->RestoreCooldownPercentage(CooldownData[Name]);
		}
	}
}

void UHAbilityComponent::OnAttackEnded(const FAttackResult& AttackResult)
{
	if (!QueuedAttackAbility)
	{
		return;
	}

	QueuedAttackAbility->OnAttackFinished(AttackResult);
	GetCaster()->GetAttackComponent()->StopAttacking();

	OnCastPointFinished.Broadcast(QueuedAttackAbility);
	QueuedAttackAbility = nullptr;
}

float UHAbilityComponent::GetCastPoint(UHSpellAbility* Ability) const
{
	float CastPoint = Ability->GetCastPoint();
	bool bIsChilled = GetCaster()->GetStatusEffectComponent()->IsConditionActive(EStatusCondition::Chilled);
	return bIsChilled ? CastPoint * ChilledCastTimeMultiplier : CastPoint;
}

UAnimMontage* UHAbilityComponent::GetCastAnimation(UHSpellAbility* Ability) const
{
	UAnimMontage* CustomAnimation = Ability->GetCastAnimation();
	return CustomAnimation ? CustomAnimation : DefaultCastAnimation;
}


// Action bar component

UHActionBarComponent::UHActionBarComponent()
{
}

UHAbility* UHActionBarComponent::GetAbilityByIndex(int32 Index) const
{
	if (Index >= EquippedAbilities.Num())
	{
		return nullptr;
	}

	return EquippedAbilities[Index];
}

void UHActionBarComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AbilityComponent = Cast<UHAbilityComponent>(GetOwner()->GetComponentByClass(UHAbilityComponent::StaticClass()));
	if (!AbilityComponent.IsValid())
	{
		UE_LOG(LogAbility, Log, TEXT("ActionBarComponent owner has no AbilityComponent"));
		return;
	}

	TArray<UHAbility*> AllAbilities = AbilityComponent->GetAbilities();
	EquippedAbilities.Init(nullptr, MaxAbilities);

	AHPlayerCharacter* Character = Cast<AHPlayerCharacter>(GetOwner());
	TArray<FString> SavedAbilities = Character->GetSaveData()->EquippedAbilities;
	if (SavedAbilities.Num() == 0)
	{
		SetDefaultActionBar(AllAbilities);
	}
	else
	{
		LoadActionBar(AllAbilities, SavedAbilities);
	}
}

void UHActionBarComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SaveActionBar();
}

void UHActionBarComponent::SetDefaultActionBar(const TArray<UHAbility*>& AllAbilities)
{
	int32 ForLimit = FMath::Min(AllAbilities.Num(), MaxAbilities);
	for (int32 i = 0; i < ForLimit; i++)
	{
		EquippedAbilities[i] = AllAbilities[i];
	}
}

void UHActionBarComponent::LoadActionBar(TArray<UHAbility*>& AllAbilities, const TArray<FString>& SavedAbilities)
{
	int32 ForLimit = FMath::Min(SavedAbilities.Num(), MaxAbilities);
	for (uint8 i = 0; i < ForLimit; i++)
	{
		FString AbilityName = SavedAbilities[i];
		UHAbility** FoundAbilityPtr = AllAbilities.FindByPredicate([AbilityName](UHAbility* Ability)
																   {
																	   return Ability->GetClass()->GetName() == AbilityName;
																   });
		EquippedAbilities[i] = FoundAbilityPtr ? *FoundAbilityPtr : nullptr;
	}
}

void UHActionBarComponent::SaveActionBar()
{
	AHPlayerCharacter* Character = Cast<AHPlayerCharacter>(GetOwner());
	UHPlayerCharacterSave* SaveData = Character->GetSaveData();
	// clear old ability array and fill it with up-to-date data
	SaveData->EquippedAbilities.Empty(MaxAbilities);
	for (UHAbility* Ability : EquippedAbilities)
	{
		SaveData->EquippedAbilities.Add(GetAbilityClassName(Ability));
	}
}

void UHActionBarComponent::UseAbilityByIndex(int32 Index)
{
	UHAbility* Ability = GetAbilityByIndex(Index);
	if (!Ability)
	{
		return;
	}

	if (AbilityComponent.IsValid())
	{
		AbilityComponent->UseAbility(Ability);
	}
}

void UHActionBarComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	const FName PropertyName = PropertyChangedEvent.GetPropertyName();
	if (PropertyName == GET_MEMBER_NAME_CHECKED(UHActionBarComponent, MaxAbilities))
	{
		// resize ability array to new value
		EquippedAbilities.SetNum(MaxAbilities);
	}
}

bool UHActionBarComponent::SetActionBarAbility(UHAbility* Ability, int32 Index)
{
	if (!AbilityComponent.IsValid())
	{
		UE_LOG(LogAbility, Error, TEXT("ActionBarComponent has invalid reference to AbilityComponent"));
		return false;
	}
	
	if (Ability && !AbilityComponent->HasAbility(Ability))
	{
		UE_LOG(LogAbility, Error, TEXT("Trying to add ability %s which is unavailable to this character"), *Ability->GetSkillNameAsString());
		return false;
	}

	if (Index >= EquippedAbilities.Num())
	{
		UE_LOG(LogAbility, Error, TEXT("Invalid index %d for action bar ability"), Index);
		return false;
	}

	EquippedAbilities[Index] = Ability;
	return true;
}
