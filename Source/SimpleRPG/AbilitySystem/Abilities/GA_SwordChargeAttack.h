// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeGameplayAbility.h"
#include "GA_SwordChargeAttack.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLERPG_API UGA_SwordChargeAttack : public UMeleeGameplayAbility
{
	GENERATED_BODY()

public:
	
	UGA_SwordChargeAttack();

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

	/* AbilityTask_PlayMontageAndWait */
	void PlayMontage();

	/* AbilityTask_WaitInputReleased */
	void WaitInputRelease();

	UFUNCTION()
	void WaitInputReleaseCallBack(float TimeHeld);

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();

	//Damage
private:
	
	void DamageEventTask();

	UFUNCTION()
	void OnDamageEvent(FGameplayEventData PayLoad);

public:

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|ChargeAttack")
	TArray<FName> SectionNames;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|ChargeAttack")
	TObjectPtr<UAnimMontage> ChargeAttackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|ChargeAttack")
	TSubclassOf<UGameplayEffect> UnderChargeAttackEffect;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|ChargeAttack")
	TSubclassOf<UGameplayEffect> FullChargeAttackEffect;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|ChargeAttack")
	float FullChargeTime = 2.0f;

private:

	FActiveGameplayEffectHandle UnderChargeAttackEffectHandle;

	FActiveGameplayEffectHandle FullChargeAttackEffectHandle;


	//Relate Damage
public:

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|DamageEffect")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Damage")
	FDataTableRowHandle FullChargeRow;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Damage")
	FDataTableRowHandle UnderChargeRow;


	bool bIsFullCharge = false;
};
