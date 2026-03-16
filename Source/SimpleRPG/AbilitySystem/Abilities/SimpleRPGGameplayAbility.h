// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "SimpleRPGGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLERPG_API USimpleRPGGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	

public:

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Ability")
	FGameplayTag InputTag;
};
