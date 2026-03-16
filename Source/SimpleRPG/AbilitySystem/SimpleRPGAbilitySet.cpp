// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGAbilitySet.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "../AbilitySystem/Abilities/SimpleRPGGameplayAbility.h"

void USimpleRPGAbilitySet::GiveToAbilitySystem(UAbilitySystemComponent* ASC) const
{
	if (!IsValid(ASC))
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, AbilitySystemComponent is invalid"), __LINE__, __FUNCTION__);
		return;
	}

	for (const TSubclassOf<USimpleRPGGameplayAbility>& AbilityClass : GrantedAbilities)
	{
		if (!IsValid(AbilityClass))
		{
			continue;
		}

		FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
}
