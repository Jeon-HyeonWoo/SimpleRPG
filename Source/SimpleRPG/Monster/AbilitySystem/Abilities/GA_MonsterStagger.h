// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleRPG/AbilitySystem/Abilities/SimpleRPGGameplayAbility.h"
#include "GA_MonsterStagger.generated.h"

/**
 * 
 */

class UAbilityTask_PlayMontageAndWait;

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
	void TaskWaitForStaggerEvent();

private:
	//Callback Function
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();

	UFUNCTION()
	void OnMontageInterupted();

	UFUNCTION()
	void OnStaggeredAgain(FGameplayEventData PayLoad);

public:

	//현재 재생 중인 Montage Task를 보관 (재 시작시 이전 Task 정리용)
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;

	//WaitGameplayEventTag용 태크
	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Tag")
	FGameplayTag StaggerEventTag;


};
