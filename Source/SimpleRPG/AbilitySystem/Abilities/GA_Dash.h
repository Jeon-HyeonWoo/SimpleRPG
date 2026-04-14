// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleRPGGameplayAbility.h"
#include "GA_Dash.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLERPG_API UGA_Dash : public USimpleRPGGameplayAbility
{
	GENERATED_BODY()
	
public:

	UGA_Dash();

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

	virtual UGameplayEffect* GetCooldownGameplayEffect() const override;

private:

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();

	FVector GetDashDirection() const;

private:

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Dash")
	TObjectPtr<UAnimMontage> DashMontage;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Dash")
	float DashDistance = 1200.0f;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Dash")
	float DashDuration;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Dash")
	TSubclassOf<UGameplayEffect> CoolDownEffect;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Dash")
	TSubclassOf<UGameplayEffect> BlockMovementEffect;

	FActiveGameplayEffectHandle BlockMovementEffectHandle;
	
};
