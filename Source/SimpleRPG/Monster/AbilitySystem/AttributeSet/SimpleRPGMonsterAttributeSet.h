// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "SimpleRPGMonsterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMonsterHPDepleted, AActor*, Instigator);

UCLASS()
class SIMPLERPG_API USimpleRPGMonsterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data);

public:
	
	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData MaxHP;
	ATTRIBUTE_ACCESSORS(USimpleRPGMonsterAttributeSet, MaxHP)

	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData HP;
	ATTRIBUTE_ACCESSORS(USimpleRPGMonsterAttributeSet, HP)

	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData AD_AttackPower;
	ATTRIBUTE_ACCESSORS(USimpleRPGMonsterAttributeSet, AD_AttackPower)

	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData AP_AttackPower;
	ATTRIBUTE_ACCESSORS(USimpleRPGMonsterAttributeSet, AP_AttackPower)

	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(USimpleRPGMonsterAttributeSet, AttackSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData AD_Defense;
	ATTRIBUTE_ACCESSORS(USimpleRPGMonsterAttributeSet, AD_Defense)

	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData AP_Defense;
	ATTRIBUTE_ACCESSORS(USimpleRPGMonsterAttributeSet, AP_Defense)

	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData CriticalChance;
	ATTRIBUTE_ACCESSORS(USimpleRPGMonsterAttributeSet, CriticalChance)

	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData CriticalMultiplier;
	ATTRIBUTE_ACCESSORS(USimpleRPGMonsterAttributeSet, CriticalMultiplier)

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData MovementSpeedMultiplier;
	ATTRIBUTE_ACCESSORS(USimpleRPGMonsterAttributeSet, MovementSpeedMultiplier)

	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(USimpleRPGMonsterAttributeSet, Damage)
	
	
public:

	UPROPERTY(BlueprintAssignable) 
	FOnMonsterHPDepleted OnHPDepleted;
};
