// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleRPGGameplayAbility.h"
#include "PlayerGameplayAbility.h"
#include "GA_Dash.generated.h"

UENUM()
enum class EDashDirection : uint8
{
	Forward,
	ForwardRight,
	Right,
	BackwardRight,
	Backward,
	BackwardLeft,
	Left,
	ForwardLeft
};

/**
 * 
 */
UCLASS()
class SIMPLERPG_API UGA_Dash : public UPlayerGameplayAbility
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

private:

	UFUNCTION()
	void OnMontageCompleted();

	UFUNCTION()
	void OnMontageCancelled();

	FVector GetDashDirection() const;

	EDashDirection GetDashDirectionEnum() const;

	void PlayMontage();

private:

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Dash")
	TMap<EDashDirection, TObjectPtr<UAnimMontage>> DashMontage;
};
