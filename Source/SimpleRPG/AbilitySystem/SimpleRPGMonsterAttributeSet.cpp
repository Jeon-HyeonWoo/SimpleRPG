// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGMonsterAttributeSet.h"
#include "GameplayEffectExtension.h"

void USimpleRPGMonsterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		SetHP(FMath::Clamp(GetHP(), 0.0f, GetMaxHP()));

		if (GetHP() <= 0.0f)
		{
			OnHPDepleted.Broadcast(Data.EffectSpec.GetContext().GetInstigator());
		}
	}

}
