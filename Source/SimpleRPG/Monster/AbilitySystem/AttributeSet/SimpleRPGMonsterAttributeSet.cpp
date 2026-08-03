// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGMonsterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "SimpleRPG/Monster/SimpleRPGMonsterBase.h"
#include "Perception/AISenseConfig_Damage.h"
#include "SimpleRPG/Monster/Components/MonsterHealthComponent.h"

void USimpleRPGMonsterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		HandleIncomingDamage(Data);
	}

}

void USimpleRPGMonsterAttributeSet::HandleIncomingDamage(const FGameplayEffectModCallbackData& Data)
{
	//1. IncomingDamage를 Local 변수에 저장
	const float LocalDamage = GetIncomingDamage();
	
	//2. IncomingDamage 초기화
	SetIncomingDamage(0.0f);

	//3. Local Damage가 0 이하면 적용할 필요가 없으니 return
	if (LocalDamage <= 0.0f) { return; }

	//4. 실제 HP 차감
	SetHP(FMath::Clamp(GetHP() - LocalDamage, 0.0f, GetMaxHP()));

	//5. 공격자 추출 및 유효성 검사
	AActor* Instigator = Data.EffectSpec.GetContext().GetInstigator();
	if (!IsValid(Instigator))
	{
		UE_LOG(LogTemp, Warning, TEXT("Instigator invalid : %d, %hs"), __LINE__, __FUNCTION__);
	}

	//6. 사망 및 피격 분기
	if (GetHP() <= 0.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("%d, %hs"), __LINE__, __FUNCTION__);
		OnHPDepleted.Broadcast(Instigator);
	}
	else
	{
		ASimpleRPGMonsterBase* Monster = Cast<ASimpleRPGMonsterBase>(GetOwningActor());
		if (!ensureMsgf(IsValid(Monster), TEXT("MonsterCharacter invalid")))
		{
			return;
		}

		UMonsterHealthComponent* MonsterHealthComp = Monster->GetMonsterHealthComponent();

		if (IsValid(MonsterHealthComp))
		{
			MonsterHealthComp->HandleDamaged(Instigator);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Monster cast failed : %d, %hs"), __LINE__, __FUNCTION__);
		}
		
	}
}
