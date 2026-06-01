// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGAttributeSet.h"
#include "GameplayEffectExtension.h"

void USimpleRPGAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.0f, GetMaxHealth()));

		if (GetHealth() <= 0.0f)
		{
			OnPlayerHealthDepleted.Broadcast(Data.EffectSpec.GetContext().GetInstigator());
		}
	}
}

