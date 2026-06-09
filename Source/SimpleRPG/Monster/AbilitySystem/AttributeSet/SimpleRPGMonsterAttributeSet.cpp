// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGMonsterAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "SimpleRPG/Monster/SimpleRPGMonsterBase.h"
#include "Perception/AISenseConfig_Damage.h"

void USimpleRPGMonsterAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetHPAttribute())
	{
		SetHP(FMath::Clamp(GetHP(), 0.0f, GetMaxHP()));

		//Get Casted DamagedActor(Monster, this)
		ASimpleRPGMonsterBase* Monster = Cast<ASimpleRPGMonsterBase>(GetOwningActor());
		if (!Monster)
		{
			UE_LOG(LogTemp, Error, TEXT("Casted Monster is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
			return;
		}

		//Get Instigator(Player)
		AActor* Instigator = Data.EffectSpec.GetContext().GetInstigator();
		if (!Instigator)
		{
			UE_LOG(LogTemp, Error, TEXT("instigator is nullptr : %d, %hs"), __LINE__, __FUNCTION__);
		}
		else
		{
			//UpdateHP, Report DamageSense to Monster
			Monster->HandleDamaged(Instigator);
		}

		if (GetHP() <= 0.0f)
		{
			OnHPDepleted.Broadcast(Instigator);
		}
	}

}
