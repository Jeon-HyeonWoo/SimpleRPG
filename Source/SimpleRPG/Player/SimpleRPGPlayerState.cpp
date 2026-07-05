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

}

const UPawnData* ASimpleRPGPlayerState::GetPawnData() const
{
	if (!ensureMsgf(IsValid(PawnData), TEXT("PlayerState : PawnData invalid")))
	{
		return nullptr;
	}

	return PawnData;
}

void ASimpleRPGPlayerState::InitializeStats()
{
	//1.Get AS and valid check
	USimpleRPGAttributeSet* AS = GetAttributeSet();
	if (!ensureMsgf(IsValid(AS), TEXT("PlayerState : AS invalid")))
	{
		return;
	}

	const UPawnData* PD = GetPawnData();
	if (!IsValid(PD)) return;

	FPlayerStat PlayerStat = PD->DefaultStats;

	AS->InitMaxHP(PlayerStat.MaxHP);
	AS->InitHP(PlayerStat.MaxHP);

	AS->InitMaxMP(PlayerStat.MaxMP);
	AS->InitMP(PlayerStat.MaxMP);

	AS->InitAD_AttackPower(PlayerStat.AD_AttackPower);
	AS->InitAP_AttackPower(PlayerStat.AP_AttackPower);

	AS->InitAttackSpeed(PlayerStat.AttackSpeed);

	AS->InitCriticalChance(PlayerStat.CriticalChance);
	AS->InitCriticalMultiplier(PlayerStat.CriticalMultiplier);

	AS->InitAD_Defense(PlayerStat.AD_Defense);
	AS->InitAP_Defense(PlayerStat.AP_Defense);

	AS->InitMoveSpeedMutiplier(PlayerStat.MoveSpeedMutiplier);


}