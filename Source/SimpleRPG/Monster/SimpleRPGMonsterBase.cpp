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



ASimpleRPGMonsterBase::ASimpleRPGMonsterBase()
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
}

void ASimpleRPGMonsterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);

	if (!IsValid(MonsterData))
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterData is unvalid %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	MonsterData->AbilitySet->GiveToAbilitySystem(GetAbilitySystemComponent());

	if (!IsValid(MonsterData->InitStatsEffect))
	{
		UE_LOG(LogTemp, Error, TEXT("MontserData->InitStatsEffect is unvalid, %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	//초기 스탯 부여
	GetAbilitySystemComponent()->ApplyGameplayEffectToSelf(
		MonsterData->InitStatsEffect.GetDefaultObject(),	//GameplayEffect CDO
		1.0f,												//Level
		GetAbilitySystemComponent()->MakeEffectContext()	//Context
		);
	
	//Test용 Death 함수 바인딩
	AttributeSet->OnHPDepleted.AddDynamic(this, &ASimpleRPGMonsterBase::HandleDeath);
	
}

void ASimpleRPGMonsterBase::HandleDeath(AActor* Actor)
{
	if (bIsDead) 
	{
		return;
	}
	bIsDead = true;
	Destroy();

}

void ASimpleRPGMonsterBase::UpdateHPBar()
{
	UMonsterHPBarWidget* W_MonsterHPBar = Cast<UMonsterHPBarWidget>(HPBarWidgetComponent->GetWidget());
	if (!W_MonsterHPBar)
	{
		UE_LOG(LogTemp, Error, TEXT("MonsterHPBar is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		return;
	}

	W_MonsterHPBar->SetHP(GetAttributeSet()->GetHP(), GetAttributeSet()->GetMaxHP());
}
