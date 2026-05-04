// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGMonsterBase.h"
#include "AbilitySystemComponent.h"
#include "../AbilitySystem/SimpleRPGMonsterAttributeSet.h"


ASimpleRPGMonsterBase::ASimpleRPGMonsterBase()
{
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
	
}

void ASimpleRPGMonsterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);

	//AttributeSet 초기 값은 각 객체에 맞게 설정
}
