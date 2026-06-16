// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGPlayerState.h"
#include "../Character/PawnData.h"
#include "AbilitySystemComponent.h"
#include "../AbilitySystem/SimpleRPGAbilitySet.h"
#include "../AbilitySystem/SimpleRPGAttributeSet.h"

ASimpleRPGPlayerState::ASimpleRPGPlayerState(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	//Create AbilitySystemComponent
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	if(IsValid(AbilitySystemComponent))
	{
		UE_LOG(LogTemp, Log, TEXT("ASC Created Successfully"));
	}

	//CreateAttributeSet
	AttributeSet = CreateDefaultSubobject<USimpleRPGAttributeSet>(TEXT("AttributeSet"));

	if (!IsValid(AttributeSet))
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs AttributeSet Create Failed"), __LINE__, __FUNCTION__);
	}
}

UAbilitySystemComponent* ASimpleRPGPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

void ASimpleRPGPlayerState::SetPawnData(UPawnData* InPawnData)
{
	if (!IsValid(InPawnData))
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, InPawnData is invalid"), __LINE__, __FUNCTION__);
		return;
	}

	PawnData = InPawnData;

	if (!IsValid(PawnData->AbilitySet))
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, AbilitySet in PawnData is invalid"), __LINE__, __FUNCTION__);
		return;
	}
	PawnData->AbilitySet->GiveToAbilitySystem(AbilitySystemComponent);
	
}

void ASimpleRPGPlayerState::InitializeStats()
{
	//1.Get AS and valid check
	USimpleRPGAttributeSet* AS = GetAttributeSet();
	if (!ensureMsgf(IsValid(AS), TEXT("PlayerState : AS invalid")))
	{
		return;
	}
	
	//2. Init Stat
	AS->InitMovementSpeed(600.0f);
	AS->InitHealth(100.0f);
	AS->InitMaxHealth(100.0f);
	AS->InitPower(10.0f);
	AS->InitMp(100.0f);
	AS->InitMaxMp(100.0f);
	
}
