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

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerHPDepleted, AActor*, Instigator);

UCLASS()
class SIMPLERPG_API USimpleRPGAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

public:

	UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData MaxHP;
	ATTRIBUTE_ACCESSORS(USimpleRPGAttributeSet, MaxHP)

		UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData HP;
	ATTRIBUTE_ACCESSORS(USimpleRPGAttributeSet, HP)

		UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData MaxMP;
	ATTRIBUTE_ACCESSORS(USimpleRPGAttributeSet, MaxMP)

		UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData MP;
	ATTRIBUTE_ACCESSORS(USimpleRPGAttributeSet, MP)

		UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData AD_AttackPower;
	ATTRIBUTE_ACCESSORS(USimpleRPGAttributeSet, AD_AttackPower)

		UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData AP_AttackPower;
	ATTRIBUTE_ACCESSORS(USimpleRPGAttributeSet, AP_AttackPower)

		UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData AttackSpeed;
	ATTRIBUTE_ACCESSORS(USimpleRPGAttributeSet, AttackSpeed)

		UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData AD_Defense;
	ATTRIBUTE_ACCESSORS(USimpleRPGAttributeSet, AD_Defense)

		UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData AP_Defense;
	ATTRIBUTE_ACCESSORS(USimpleRPGAttributeSet, AP_Defense)

		UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData CriticalChance;
	ATTRIBUTE_ACCESSORS(USimpleRPGAttributeSet, CriticalChance)

		UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData CriticalMultiplier;
	ATTRIBUTE_ACCESSORS(USimpleRPGAttributeSet, CriticalMultiplier)

		UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData MoveSpeedMutiplier;
	ATTRIBUTE_ACCESSORS(USimpleRPGAttributeSet, MoveSpeedMutiplier)

		UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FGameplayAttributeData BaseMoveSpeed;
	ATTRIBUTE_ACCESSORS(USimpleRPGAttributeSet, BaseMoveSpeed)

		UPROPERTY(BlueprintReadOnly, Category = "Stat")
	FGameplayAttributeData InComingDamage;
	ATTRIBUTE_ACCESSORS(USimpleRPGAttributeSet, InComingDamage)

public:

	UPROPERTY(BlueprintAssignable)
	FOnPlayerHPDepleted OnHPDepleted;
};
