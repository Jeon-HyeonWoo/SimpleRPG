// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleRPGGameplayAbility.h"
#include "GA_SwordAttack.generated.h"

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

	//Controlled Combo Window (Called by AnimNotify), NofityState의 Open, Close를 담당하는 함수
public:

	UFUNCTION(BlueprintCallable, Category = "Combo")
	void OpenComboWindow();

	UFUNCTION(BlueprintCallable, Category = "Combo")
	void CloseComboWindow();

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

private:

	void StartNextCombo();
	void ResetCombo();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	TObjectPtr<UAnimMontage> ComboMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	TArray<FName> ComboSectionNames;

	UPROPERTY(EditDefaultsOnly, Category = "Combo")
	int32 MaxComboCount = 3;

	UPROPERTY(BlueprintReadOnly, Category = "Combo")
	int32 CurrentComboCount = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Combo")
	bool bComboWindowOpen = false;

	UPROPERTY(BlueprintReadOnly, Category = "Combo")
	bool bNextComboQueued = false;


};
