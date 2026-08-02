// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "SimpleRPG/Character/SimpleRPGPlayerCharacter.h"
#include "SimpleRPG/SimpleRPGGameplayTag.h"
#include "AbilitySystemBlueprintLibrary.h"

void USimpleRPGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleInComingDamage(Data);
	}
}

void USimpleRPGAttributeSet::HandleInComingDamage(const FGameplayEffectModCallbackData& Data)
{
	//1. IncomingDamage를 Loacl에 저장
	const float LocalDamage = GetIncomingDamage();

	//2. IncomingDamage 초기화
	SetIncomingDamage(0.0f);

	//3. Local Damage(실제 받는 데미지)가 0이면 return;
	if (LocalDamage <= 0.0f) return;

	//3. 실제 HP차감 (현재 HP에서 로컬에 저장된 Damage 차감)
	SetHP(FMath::Clamp(GetHP() - LocalDamage, 0.0f, GetMaxHP()));

	//4. 공격자(Monster)추출 및 유효성 검사
	AActor* Instigator = Data.EffectSpec.GetContext().GetInstigator();
	if (!IsValid(Instigator))
	{
		UE_LOG(LogTemp, Warning, TEXT("Instigator invalid : %d, %hs"), __LINE__, __FUNCTION__);
	}

	//5. 사망 및 피격 분기
	if (GetHP() <= 0.0f)
	{
		OnPlayerHPDepleted.Broadcast(Instigator);
	}
	else
	{
		FGameplayTagContainer OutputTags;
		Data.EffectSpec.GetAllAssetTags(OutputTags);
		
		OnPlayerDamaged.Broadcast(Instigator, OutputTags);
	}

}

