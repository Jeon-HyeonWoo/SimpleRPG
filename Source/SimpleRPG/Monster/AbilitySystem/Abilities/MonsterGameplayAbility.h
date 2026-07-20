// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleRPG/AbilitySystem/Abilities/SimpleRPGGameplayAbility.h"
#include "SimpleRPG/Monster/Data/MonsterActionData.h"
#include "MonsterGameplayAbility.generated.h"

/**
 * 
 */
class ASimpleRPGMonsterBase;
class UAbilityTask_PlayMontageAndWait;

UCLASS()
class SIMPLERPG_API UMonsterGameplayAbility : public USimpleRPGGameplayAbility
{
	GENERATED_BODY()
	
protected:

	ASimpleRPGMonsterBase* GetOwningMonster() const;
	const FMonsterMontageEntry* GetRandomMontageEntry(const FGameplayTag& ActionTag) const;

	bool PlayActionMontage(const FGameplayTag& ActionTag);

	bool GetMonsterSkillRatio(const FDataTableRowHandle& RowHandle, float& OutRatio) const;

	UFUNCTION()
	void HandleMontageCompleted();
	UFUNCTION()
	void HandleMontageCancelled();

	virtual void OnActionMontageCompleted();
	virtual void OnActionMontageCancelled();

protected:

	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;

	const FMonsterMontageEntry* CurrentEntry = nullptr;
};
