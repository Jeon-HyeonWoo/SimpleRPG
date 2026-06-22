// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalc_Damage.h"
#include "SimpleRPG/AbilitySystem/SimpleRPGAttributeSet.h"
#include "SimpleRPG/Monster/AbilitySystem/AttributeSet/SimpleRPGMonsterAttributeSet.h"

struct FDamageStatics
{
#pragma region Source Attribute

	FGameplayEffectAttributeCaptureDefinition AD_AttackPowerDef;

	FGameplayEffectAttributeCaptureDefinition CriticalChanceDef;
	FGameplayEffectAttributeCaptureDefinition CriticalMultiplierDef;

#pragma endregion


#pragma region Target Attribute

	FGameplayEffectAttributeCaptureDefinition AD_DefenseDef;

#pragma endregion


	FDamageStatics()
	{
		//Source
		AD_AttackPowerDef = FGameplayEffectAttributeCaptureDefinition(
			USimpleRPGAttributeSet::GetAD_AttackPowerAttribute(),			//Attribute
			EGameplayEffectAttributeCaptureSource::Source,					//°ø°ÝÀÚ
			false															//non-SnapShot
		);

		CriticalChanceDef = FGameplayEffectAttributeCaptureDefinition(
			USimpleRPGAttributeSet::GetCriticalChanceAttribute(),
			EGameplayEffectAttributeCaptureSource::Source,
			false
		);

		CriticalMultiplierDef = FGameplayEffectAttributeCaptureDefinition(
			USimpleRPGAttributeSet::GetCriticalMultiplierAttribute(),
			EGameplayEffectAttributeCaptureSource::Source,
			false
		);

		//Target
		AD_DefenseDef = FGameplayEffectAttributeCaptureDefinition(
			USimpleRPGMonsterAttributeSet::GetAD_DefenseAttribute(),
			EGameplayEffectAttributeCaptureSource::Target,
			false
		);
	}
};

static const FDamageStatics& DamageStatics()
{
	static FDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	//Added Source Catpure Value
	RelevantAttributesToCapture.Add(DamageStatics().AD_AttackPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalMultiplierDef);

	//Added Target Capture Value
	RelevantAttributesToCapture.Add(DamageStatics().AD_DefenseDef);
}
