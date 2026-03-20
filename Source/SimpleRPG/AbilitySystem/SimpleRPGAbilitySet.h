// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../Input/SimpleRPGAbilityInputID.h"
#include "SimpleRPGAbilitySet.generated.h"

/**
 * 
 */

class USimpleRPGGameplayAbility;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FSimpleRPGAbilitySetEntry
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USimpleRPGGameplayAbility> AbilityClass;

	UPROPERTY(EditDefaultsOnly)
	ESimpleRPGAbilityInputID InputID = ESimpleRPGAbilityInputID::None;
};

UCLASS()
class SIMPLERPG_API USimpleRPGAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	void GiveToAbilitySystem(UAbilitySystemComponent* ASC) const;

public:

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Abilities")
	TArray<FSimpleRPGAbilitySetEntry> GrantedAbilities;
	 
};
