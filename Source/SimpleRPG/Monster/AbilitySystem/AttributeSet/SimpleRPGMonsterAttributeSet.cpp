// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGMonsterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "SimpleRPG/Monster/SimpleRPGMonsterBase.h"

void USimpleRPGMonsterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		SetHP(FMath::Clamp(GetHP(), 0.0f, GetMaxHP()));

		ASimpleRPGMonsterBase* Monster = Cast<ASimpleRPGMonsterBase>(GetOwningActor());
		if (!Monster)
		{
			UE_LOG(LogTemp, Error, TEXT("Casted Monster is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
			return;
		}

		Monster->UpdateHPBar();

		if (GetHP() <= 0.0f)
		{
			OnHPDepleted.Broadcast(Data.EffectSpec.GetContext().GetInstigator());
		}
	}

}
