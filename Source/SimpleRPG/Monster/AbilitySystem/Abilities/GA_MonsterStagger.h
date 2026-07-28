// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleRPG/Monster/AbilitySystem/Abilities/MonsterGameplayAbility.h"
#include "GA_MonsterStagger.generated.h"

/**
 * 
 */

class UAbilityTask_PlayMontageAndWait;

UCLASS()
class SIMPLERPG_API UGA_MonsterStagger : public UMonsterGameplayAbility
{
	GENERATED_BODY()
	
public:

	UGA_MonsterStagger();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* Actorinfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;
	
	
};
