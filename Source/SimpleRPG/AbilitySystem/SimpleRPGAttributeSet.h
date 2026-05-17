// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "SimpleRPGAttributeSet.generated.h"


/*
* - GAMEPLAYATTRIBUTE_PROPERTY_GETTER : FGameplayAttribute를 반환하는 Static 함수 생성
* GameplayEffect가 어떤 Attribute를 수정할지 참조할 때 사용
* - Getter, Setter, Init함수 생성
*/
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
		GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/**
 * 
 */

UCLASS()
class SIMPLERPG_API USimpleRPGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData MovementSpeed;
	ATTRIBUTE_ACCESSORS(USimpleRPGAttributeSet, MovementSpeed)

	UPROPERTY(BlueprintReadOnly, Category = "Vital")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(USimpleRPGAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Vital")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(USimpleRPGAttributeSet, MaxHealth)

	UPROPERTY(BlueprintReadOnly, Category = "Power")
	FGameplayAttributeData Power;
	ATTRIBUTE_ACCESSORS(USimpleRPGAttributeSet, Power)

	UPROPERTY(BlueprintReadOnly, Category = "Mp")
	FGameplayAttributeData Mp;
	ATTRIBUTE_ACCESSORS(USimpleRPGAttributeSet, Mp)

	UPROPERTY(BlueprintReadOnly, Category = "MaxMp")
	FGameplayAttributeData MaxMp;
	ATTRIBUTE_ACCESSORS(USimpleRPGAttributeSet, MaxMp)
};
