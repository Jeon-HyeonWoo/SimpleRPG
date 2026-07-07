// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleRPGGameplayAbility.h"
#include "GA_SwordPowerUp.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLERPG_API UGA_SwordPowerUp : public USimpleRPGGameplayAbility
{
	GENERATED_BODY()
	
public:

	UGA_SwordPowerUp();

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

	/* UAbilityTask_PlayMontageAndWait관련 */
	void PlayPowerUpMontage();

	/* UAbilityTask_WaitGameplayEvent관련 */
	void PowerUpEvent();

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();

	UFUNCTION()
	void OnPowerUpActivated(FGameplayEventData PayLoad);

public:

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|PowerUp")
	TObjectPtr<UAnimMontage> PowerUpMontage;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|PowerUp")
	TSubclassOf<UGameplayEffect> BuffEffect;

};
