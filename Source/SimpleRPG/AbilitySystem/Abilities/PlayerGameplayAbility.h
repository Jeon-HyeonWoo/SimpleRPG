// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleRPGGameplayAbility.h"
#include "PlayerGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLERPG_API UPlayerGameplayAbility : public USimpleRPGGameplayAbility
{
	GENERATED_BODY()
	
protected:

	//TODO : 추후 MMC 방법으로 Cooldown GE 관리 수정 
	virtual void ApplyCooldown(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo
	) const override;

	virtual const FGameplayTagContainer* GetCooldownTags() const override { return &CooldownTags; }

protected:

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|CoolDown")
	FDataTableRowHandle CooldownDataRow;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|CoolDown")
	FGameplayTagContainer CooldownTags;
};
