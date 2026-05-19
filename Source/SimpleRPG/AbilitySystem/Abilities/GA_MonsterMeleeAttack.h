// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleRPGGameplayAbility.h"
#include "GA_MonsterMeleeAttack.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FMonsterAttackMontageData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> AnimMontage;

	UPROPERTY(EditDefaultsOnly)
	FName DamageRowName;
};

UCLASS()
class SIMPLERPG_API UGA_MonsterMeleeAttack : public USimpleRPGGameplayAbility
{
	GENERATED_BODY()
	
public:

	UGA_MonsterMeleeAttack();

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
	
	//AbilityTask_PlayMontageAndWait 세팅
	void PlayMontage();

	//AbbilityTask_WaitGameplayEvent 세팅
	void WaitForHitEvent();

	//히트 시 데미지 적용
	UFUNCTION()
	void OnHitEventReceived(FGameplayEventData PayLoad);

	//Montage 콜백
	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();

public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Montage")
	TArray<FMonsterAttackMontageData> AttackMontageDataArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Damage")
	TSubclassOf<UGameplayEffect> DamageEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Damage")
	TObjectPtr<UDataTable> DamageDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Damage")
	FGameplayTag HitEventTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Ability")
	TSubclassOf<UGameplayEffect> BlockMovementEffect;


private:

	FActiveGameplayEffectHandle BlockMovementEffectHandle;

	int32 MontageDataIndex = 0;
};
