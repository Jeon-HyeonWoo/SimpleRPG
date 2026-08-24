// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleRPGGameplayAbility.h"
#include "PlayerGameplayAbility.h"
#include "GA_WeaponSwap.generated.h"

/**
 * 
 */

class USimpleRPGEquipmentComponent;
class UWeaponData;

UCLASS()
class SIMPLERPG_API UGA_WeaponSwap : public UPlayerGameplayAbility
{
	GENERATED_BODY()

public:

	UGA_WeaponSwap();

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

	void PlaySheathMontage();
	void PlayDrawMontage();

	UFUNCTION()
	void OnSheathCompleted();
	UFUNCTION()
	void OnSheathCencelled();

	UFUNCTION()
	void OnDrawCompleted();
	UFUNCTION()
	void OnDrawCencelled();

	void FinishSwap();

private:

	UPROPERTY()
	TObjectPtr<USimpleRPGEquipmentComponent> EquipmentComponent;

	UPROPERTY()
	TObjectPtr<const UWeaponData> PendingWeaponData;
};
