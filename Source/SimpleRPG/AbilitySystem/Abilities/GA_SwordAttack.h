// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleRPGGameplayAbility.h"
#include "GA_SwordAttack.generated.h"

USTRUCT(BlueprintType)
struct FComboStep
{
	GENERATED_BODY()

	//SectionName : Combo1, 2, 3
	UPROPERTY(EditDefaultsOnly)
	FName SectionName;

	UPROPERTY(EditDefaultsOnly)
	FDataTableRowHandle DamageRow;
};

/**
 * 
 */
UCLASS(Abstract, Blueprintable)
class SIMPLERPG_API UGA_SwordAttack : public USimpleRPGGameplayAbility
{
	GENERATED_BODY()
	
public:

	UGA_SwordAttack();

	virtual void InputPressed(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo
	) override;

	virtual void ActivateAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		const FGameplayEventData* TriggerEventData
	) override;

	virtual void EndAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		bool bReplicateEndAbility,
		bool bWasCancelled
	) override;

	//Montage Callback
public: 

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();

protected:

	UFUNCTION()
	void OnComboWindowOpen(FGameplayEventData PayLoad);

	UFUNCTION()
	void OnComboWindowClose(FGameplayEventData PayLoad);

	UFUNCTION()
	void OnDamageEvent(FGameplayEventData PayLoad);

private:
	//AbilityTask_PlayMontageAndWait
	void PlayComboMontage();

	void DamageEventTask();

	void ResetCombo();
	void ReserveNextCombo();


protected:

	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	TObjectPtr<UAnimMontage> ComboMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	TArray<FComboStep> ComboSteps;

	UPROPERTY(BlueprintReadOnly, Category = "Combo")
	int32 CurrentComboCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Combo")
	bool bComboWindowOpen = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combo")
	bool bNextComboQueued = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combo")
	bool bComboReserved = false;

	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	TSubclassOf<UGameplayEffect> BlockMovementEffect;

	FActiveGameplayEffectHandle BlockMovementEffectHandle;

public:

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|DamageEffect")
	TSubclassOf<UGameplayEffect> DamageEffect;
};
