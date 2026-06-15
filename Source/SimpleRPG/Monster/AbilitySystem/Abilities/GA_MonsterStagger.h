// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleRPG/AbilitySystem/Abilities/SimpleRPGGameplayAbility.h"
#include "GA_MonsterStagger.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLERPG_API UGA_MonsterStagger : public USimpleRPGGameplayAbility
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

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* Actorinfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled
	) override;

private:
	//Helper Function

	void TaskPlayMontageAndWait();

private:
	//Callback Function
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();

};
