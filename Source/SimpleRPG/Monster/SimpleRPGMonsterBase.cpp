// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGMonsterBase.h"
#include "AbilitySystemComponent.h"
#include "SimpleRPG/Monster/AbilitySystem/AttributeSet/SimpleRPGMonsterAttributeSet.h"
#include "../Monster/Data/MonsterData.h"
#include "SimpleRPG/Monster/AI/MonsterAIController.h"
#include "SimpleRPG/AbilitySystem/SimpleRPGAbilitySet.h"
#include "Components/WidgetComponent.h"
#include "SimpleRPG/UI/MonsterHPBarWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SimpleRPG/Monster/Data/MonsterStatRow.h"
#include "Perception/AISenseConfig_Damage.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "SimpleRPG/Monster/Components/MonsterHealthComponent.h"

ASimpleRPGMonsterBase::ASimpleRPGMonsterBase(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	//Base Setting
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0, 480, 0);
	}

	//Create ASC
	{
		AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

		if (!AbilitySystemComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("ASC created failed : %d, %hs"), __LINE__, __FUNCTION__);
			return;
		}
	}

	//Create AttributeSet
	//TODO : Create USimpleMonster AttribueteSet
	{
		AttributeSet = CreateDefaultSubobject<USimpleRPGMonsterAttributeSet>(TEXT("AttributeSet"));
		if (!AttributeSet)
		{
			UE_LOG(LogTemp, Error, TEXT("AttributeSet created failed, %d, %hs"), __LINE__, __FUNCTION__);
			return;
		}
	}

	//Set Default AIController
	AIControllerClass = AMonsterAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//CreateHPWidgetComponent
	{
		HPBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPBarWidgetComponent"));
		HPBarWidgetComponent->SetupAttachment(RootComponent);
		HPBarWidgetComponent->SetRelativeLocation(FVector(0, 0, 100));
		HPBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		HPBarWidgetComponent->SetDrawSize(FVector2D(150, 15));
	}

	//Create MonsterHealthComponent
	{
		MonsterHealthComponent = CreateDefaultSubobject<UMonsterHealthComponent>(TEXT("MonsterHealthComponent"));
		if (!MonsterHealthComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("MonsterHealthComponent created failed : %d, %hs"), __LINE__, __FUNCTION__);
			return;
		}
	}
}

void ASimpleRPGMonsterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);

	GiveAbility();
	InitializeStats();
	MonsterHealthComponent->InitializeWithOwner(this);
}

void ASimpleRPGMonsterBase::StopMovement()
{
	GetCharacterMovement()->StopMovementImmediately();
}

void ASimpleRPGMonsterBase::ApplyInvulnerability()
{
	//1. ASC 유효성 검사
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		UE_LOG(LogTemp, Error, TEXT("ASC is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//2.EffectContext만들기
	FGameplayEffectContextHandle ContextHandle = ASC->MakeEffectContext();
	if (!ContextHandle.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("ContextHandle is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//3. EffectSpecHandle 만들기
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(InvulnerabilityEffect, 1.0f, ContextHandle);
	if (!SpecHandle.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("SpecHandle is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//4. EffectHandle에 저장(추후 제거용)
	InvulnerabilityEffectHandle = ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
	
}

void ASimpleRPGMonsterBase::RemoveInvulnerability()
{
	UAbilitySystemComponent* ASC = GetAbilitySystemComponent();
	if (!IsValid(ASC))
	{
		UE_LOG(LogTemp, Error, TEXT("ASC is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	ASC->RemoveActiveGameplayEffect(InvulnerabilityEffectHandle);
}

void ASimpleRPGMonsterBase::GiveAbility()
{
	if (!IsValid(MonsterData))
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterData is invalid %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	if (!IsValid(MonsterData->AbilitySet))
	{
		UE_LOG(LogTemp, Error, TEXT("AbilitySet is invalid %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	MonsterData->AbilitySet->GiveToAbilitySystem(GetAbilitySystemComponent());
}

void ASimpleRPGMonsterBase::InitializeStats()
{
	//1. AttributeSet 유효성 검사
	if (!IsValid(AttributeSet))
	{
		UE_LOG(LogTemp, Error, TEXT("AttributeSet is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//2. MonsterData 유효성 검사
	if (!IsValid(MonsterData))
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterData is invalid : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}
	
	//3. StatRow 가져오기 및 검사
	const FMonsterStatRow* StatRow =
		MonsterData->StatRowHandle.GetRow<FMonsterStatRow>(TEXT("InitializeStats"));
	if (!StatRow) 
	{
		UE_LOG(LogTemp, Error, TEXT("StatRow not found : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//4. AttributeSet에 StatRow값 Initialize 적용
	{
		AttributeSet->InitMaxHP(StatRow->MaxHP);
		AttributeSet->InitHP(StatRow->MaxHP);
		AttributeSet->InitAD_AttackPower(StatRow->AD_AttackPower);
		AttributeSet->InitAP_AttackPower(StatRow->AP_AttackPower);
		AttributeSet->InitAD_Defense(StatRow->AD_Defense);
		AttributeSet->InitAP_Defense(StatRow->AP_Defense);
		AttributeSet->InitAttackSpeed(StatRow->AttackSpeed);
		AttributeSet->InitCriticalChance(StatRow->CriticalChance);
		AttributeSet->InitCriticalMultiplier(StatRow->CriticalMultiplier);
	}
	
	//5. Runtime debug 
	UE_LOG(LogTemp, Warning, TEXT("[%s] MaxHp = %.1f, AD_AP=%.1f"), *GetName(), AttributeSet->GetMaxHP(), AttributeSet->GetAD_AttackPower());
}
