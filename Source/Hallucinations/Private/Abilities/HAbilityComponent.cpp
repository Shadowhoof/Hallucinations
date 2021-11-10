// Fill out your copyright notice in the Description page of Project Settings.


#include "Abilities/HAbilityComponent.h"
#include "Abilities/HAbility.h"
#include "Abilities/HAttackAbility.h"
#include "Abilities/HSpellAbility.h"
#include "Characters/HCharacter.h"
#include "Core/HLogCategories.h"
#include "Core/HSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "Utils/HLogUtils.h"
#include "Characters/HPlayerCharacter.h"
#include "Components/HFollowComponent.h"
#include "Utils/HEnumTools.h"

DEFINE_LOG_CATEGORY(LogAbility);

// Ability component

const float UHAbilityComponent::ChilledCastTimeMultiplier = 2.f;

UHAbilityComponent::UHAbilityComponent()
{
}


bool UHAbilityComponent::CanUseAbility(UHAbility* Ability, const FAbilityTargetParameters& TargetParams) const
{
	return !GetCaster()->IsBusy() && Ability->CanBeUsed(TargetParams);
}

void UHAbilityComponent::UseSpellAbility(UHAbility* BaseAbility)
{
	ensure(BaseAbility);
	UHSpellAbility* Ability = Cast<UHSpellAbility>(BaseAbility);
	if (!Ability)
	{
		UE_LOG(LogAbility, Error, TEXT("Ability %s does not inherit UHSpellAbility"), *BaseAbility->GetSkillNameAsString());
		return;
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

	UAnimMontage* CastAnimation = GetCastAnimation(Ability);
	GetCaster()->PlayAnimMontage(CastAnimation);
	FTimerDelegate Delegate = FTimerDelegate::CreateUObject(this, &UHAbilityComponent::FinishCast, Ability);
	GetWorld()->GetTimerManager().SetTimer(CastTimerHandle, Delegate, GetCastTime(Ability), false);
}

void UHAbilityComponent::UseAttackAbility(UHAbility* UncastAbility)
{
	ensure(UncastAbility);
	UHAttackAbility* Ability = Cast<UHAttackAbility>(UncastAbility);
	if (!Ability)
	{
		UE_LOG(LogAbility, Error, TEXT("Ability %s is not of Spell type"), *UncastAbility->GetSkillNameAsString());
		return;
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
	AttackComponent->AttackEndedEvent.AddDynamic(this, &UHAbilityComponent::OnAttackEnded);
}

void UHAbilityComponent::UseAbility(UHAbility* Ability)
{
	FAbilityTargetParameters TargetParams;
	TargetParams.Actor = GetTargetActor();
	TargetParams.Location = GetTargetLocation();
	if (CanUseAbility(Ability, TargetParams))
	{
		CurrentTargetParams = TargetParams;
		EAbilityType AbilityType = Ability->GetType();
		switch (AbilityType)
		{
		case EAbilityType::Spell:
			UseSpellAbility(Ability);
			break;
		case EAbilityType::Attack:
			UseAttackAbility(Ability);
			break;
		default:
			UE_LOG(LogAbility, Error, TEXT("Unknown ability type %s of ability %s"), *EnumAsString(AbilityType), *Ability->GetSkillNameAsString());
			break;
		}
	}
}

void UHAbilityComponent::FinishCast(UHSpellAbility* Ability)
{
	Ability->OnCastFinished(CurrentTargetParams);
	bIsCasting = false;
}

bool UHAbilityComponent::IsCasting() const
{
	return bIsCasting;
}

void UHAbilityComponent::Interrupt()
{
	if (bIsCasting)
	{
		GetWorld()->GetTimerManager().ClearTimer(CastTimerHandle);
		bIsCasting = false;
	}
}

TArray<UHAbility*> UHAbilityComponent::GetAbilities() const
{
	return Abilities;
}

bool UHAbilityComponent::HasAbility(UHAbility* Ability)
{
	return Ability && Abilities.Contains(Ability);
}

void UHAbilityComponent::OnAttackEnded(const FAttackResult& AttackResult)
{
	if (!QueuedAttackAbility)
	{
		return;
	}

	QueuedAttackAbility->OnAttackFinished(AttackResult);
	QueuedAttackAbility = nullptr;
	GetCaster()->GetAttackComponent()->StopAttacking();
}

float UHAbilityComponent::GetCastTime(UHSpellAbility* Ability) const
{
	UAnimMontage* CastAnimation = GetCastAnimation(Ability);
	float BaseCastTime = CastAnimation->GetSectionLength(0);
	bool bIsChilled = GetCaster()->GetStatusEffectComponent()->IsConditionActive(EStatusCondition::Chilled);
	return bIsChilled ? BaseCastTime * ChilledCastTimeMultiplier : BaseCastTime;
}

UAnimMontage* UHAbilityComponent::GetCastAnimation(UHSpellAbility* Ability) const
{
	UAnimMontage* CustomAnimation = Ability->GetCastAnimation();
	return CustomAnimation ? CustomAnimation : DefaultCastAnimation;
}


// Action bar component

const FName EmptyAbilityName = "Ability_NONE";

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

	AHPlayerCharacter* Character = Cast<AHPlayerCharacter>(GetOwner());
	UHSaveGame* SaveData = Character->GetSaveData();
	// clear old ability array and fill it with up-to-date data
	SaveData->EquippedAbilities.Empty(MaxAbilities);
	for (UHAbility* Ability : EquippedAbilities)
	{
		FString AbilityName = Ability ? Ability->GetClass()->GetName() : EmptyAbilityName.ToString();
		SaveData->EquippedAbilities.Add(AbilityName);
	}
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
