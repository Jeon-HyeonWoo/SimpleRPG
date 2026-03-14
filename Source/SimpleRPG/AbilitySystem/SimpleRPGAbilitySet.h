// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SimpleRPGAbilitySet.generated.h"

/**
 * 
 */
class UAbilitySystemComponent;
class USimpleRPGGameplayAbility;

UCLASS()
class SIMPLERPG_API USimpleRPGAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	void GiveToAbilitySystem(UAbilitySystemComponent* ASC) const;

public:

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<USimpleRPGGameplayAbility>> GrantedAbilities;
	 
};
