// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SimpleRPGMonsterAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */
UCLASS()
class SIMPLERPG_API USimpleRPGMonsterAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(USimpleRPGMonsterAttributeSet, MovementSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Vital")
	FGameplayAttributeData MaxHP;
	ATTRIBUTE_ACCESSORS(USimpleRPGMonsterAttributeSet, MaxHP)

	UPROPERTY(BlueprintReadOnly, Category = "Vital")
	FGameplayAttributeData HP;
	ATTRIBUTE_ACCESSORS(USimpleRPGMonsterAttributeSet, HP)

	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData Damage;
	ATTRIBUTE_ACCESSORS(USimpleRPGMonsterAttributeSet, Damage)
	
	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData Defense;
	ATTRIBUTE_ACCESSORS(USimpleRPGMonsterAttributeSet, Defense)
	

};
