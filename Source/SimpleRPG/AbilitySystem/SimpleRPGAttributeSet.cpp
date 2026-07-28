// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "SimpleRPG/Character/SimpleRPGPlayerCharacter.h"

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
	//5-1. 데미지를 받고 체력이 0 이하로 내려간다면 사망
	if (GetHP() <= 0.0f)
	{
		//TODO::Death 분기
	}
	//5-2. 사망하지 않고 데미지를 깎인다는 걸 알림
	else
	{
		
		UAbilitySystemComponent* ASC = GetOwningAbilitySystemComponent();
		if (!ASC)
		{
			UE_LOG(LogTemp, Warning, TEXT("GetOwningAbilitySystem Invalid : %d, %hs"), __LINE__, __FUNCTION__);
			return;
		}

		ASimpleRPGPlayerCharacter* PlayerCharacter = Cast<ASimpleRPGPlayerCharacter>(ASC->GetAvatarActor());

		if (IsValid(PlayerCharacter))
		{
			//TODO :: 피격 분기
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("PlayerCharacter Character cast failed : %d, %hs"), __LINE__, __FUNCTION__);
		}
	}

}

