// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGAbilitySet.h"
#include "AbilitySystemComponent.h"
#include "Abilities/SimpleRPGGameplayAbility.h"
#include "GameplayAbilitySpec.h"
#include "../AbilitySystem/Abilities/SimpleRPGGameplayAbility.h"

void USimpleRPGAbilitySet::GiveToAbilitySystem(UAbilitySystemComponent* ASC) const
{
	if (!IsValid(ASC))
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, AbilitySystemComponent is invalid"), __LINE__, __FUNCTION__);
		return;
	}

	for (const FSimpleRPGAbilitySetEntry& Entry : GrantedAbilities)
	{
		if (!IsValid(Entry.AbilityClass))
		{
			UE_LOG(LogTemp, Warning, TEXT("%s, is invalid"), *(Entry.AbilityClass.Get()->GetName()));
			continue;
		}

		/* Spec = 말 그대로 Ability에 대한 스펙, 정보 묶음 AbilityClass와 Level, 부여오브젝트, 런타임테그, 핸들(포인터개념)을 포함 */
		FGameplayAbilitySpec AbilitySpec(Entry.AbilityClass, 1);
		AbilitySpec.InputID = static_cast<int32>(Entry.InputID);
		ASC->GiveAbility(AbilitySpec);
	}

}
