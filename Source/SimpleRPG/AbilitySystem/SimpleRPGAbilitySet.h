// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../Input/SimpleRPGAbilityInputID.h"
#include "GameplayAbilitySpec.h"
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

/*
* GiveToAbilitySystem의 결과를 보관하는 구조체. Granted된 쪽이 소유
*/
USTRUCT(BlueprintType)
struct FSimpleRPGAbilitySet_GrantedHandles
{
	GENERATED_BODY()

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void RemoveFromAbilitySystem(UAbilitySystemComponent* ASC);

private:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;

};

UCLASS()
class SIMPLERPG_API USimpleRPGAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	void GiveToAbilitySystem(UAbilitySystemComponent* ASC, FSimpleRPGAbilitySet_GrantedHandles* OutGrantedHandles = nullptr) const;

public:

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Abilities")
	TArray<FSimpleRPGAbilitySetEntry> GrantedAbilities;
	 
};
