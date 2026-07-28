// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SimpleRPGGameplayAbility.generated.h"

/**
 * 
 */
class ASimpleRPGCharacterBase;

UCLASS()
class SIMPLERPG_API USimpleRPGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:

	virtual void ApplyCooldown(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo
	) const override;

	virtual const FGameplayTagContainer* GetCooldownTags() const override { return &CoolDownTags; }

//Helper Function
public:

	UFUNCTION(BlueprintCallable, Category = "SimpleRPG|Ability")
	ASimpleRPGCharacterBase* GetOwnerCharacter() const;

	UAbilitySystemComponent* GetSimpleRPGASC() const;

protected:

	bool GetSkillRatio(const FDataTableRowHandle& DTRowHandle, float& OutRatio) const;
	
	void ApplyDamageToTarget(AActor* Target, TSubclassOf<UGameplayEffect> DamageEffect, float SkillRatio, FGameplayTag InflictedHitReaction = FGameplayTag());

protected:

	//이동 금지 GE를 소유자에게 적용하고 핸들을 저장
	void ApplyBlockMovement();	
	// 저장된 핸들로 이동 금지 GE를 해제
	void RemoveBlockMovement();	


protected:

	// 이동 금지 GE. 자식 GA가 Editor에서 GE_BlockMovement지정
	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Movement")
	TSubclassOf<UGameplayEffect> BlockMovementEffect;

	// 적용된 이동 금지 GE 핸들. Remove시 사용
	FActiveGameplayEffectHandle BlockMovementEffectHandle;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|CoolDown")
	FGameplayTagContainer CoolDownTags;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|CoolDown")
	FDataTableRowHandle CoolDownDataRow;
};
