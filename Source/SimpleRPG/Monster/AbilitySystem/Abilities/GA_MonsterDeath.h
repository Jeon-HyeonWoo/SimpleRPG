// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleRPG/AbilitySystem/Abilities/SimpleRPGGameplayAbility.h"
#include "GA_MonsterDeath.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLERPG_API UGA_MonsterDeath : public USimpleRPGGameplayAbility
{
	GENERATED_BODY()
	
public:

	UGA_MonsterDeath();

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

private:

	void TaskPlayDeathMontage();
	UAnimMontage* GetDeathMontage();

private:

	UFUNCTION()
	void OnMontageCompleted();
};
