// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGPlayerState.h"
#include "../Character/PawnData.h"
#include "AbilitySystemComponent.h"
#include "../AbilitySystem/SimpleRPGAbilitySet.h"

ASimpleRPGPlayerState::ASimpleRPGPlayerState(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));

	if(IsValid(AbilitySystemComponent))
	{
		UE_LOG(LogTemp, Log, TEXT("ASC Created Successfully"));
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
