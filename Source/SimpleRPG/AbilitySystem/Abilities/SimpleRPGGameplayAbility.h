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
	

//Helper Function
public:

	UFUNCTION(BlueprintCallable, Category = "SimpleRPG|Ability")
	ASimpleRPGCharacterBase* GetOwnerCharacter() const;

	UAbilitySystemComponent* GetSimpleRPGASC() const;
};
