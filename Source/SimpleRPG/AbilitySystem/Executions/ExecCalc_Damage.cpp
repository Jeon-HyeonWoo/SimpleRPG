// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecCalc_Damage.h"
#include "SimpleRPG/SimpleRPGGameplayTag.h"
#include "SimpleRPG/AbilitySystem/SimpleRPGAttributeSet.h"
#include "SimpleRPG/Monster/AbilitySystem/AttributeSet/SimpleRPGMonsterAttributeSet.h"

struct FDamageStatics
{
#pragma region Source Attribute

	FGameplayEffectAttributeCaptureDefinition AD_AttackPowerDef;

	FGameplayEffectAttributeCaptureDefinition CriticalChanceDef;
	FGameplayEffectAttributeCaptureDefinition CriticalMultiplierDef;

	FGameplayEffectAttributeCaptureDefinition DamageToMonsterDef;
#pragma endregion


#pragma region Target Attribute

	FGameplayEffectAttributeCaptureDefinition AD_DefenseDef;

#pragma endregion


	FDamageStatics()
	{
		//Source
		AD_AttackPowerDef = FGameplayEffectAttributeCaptureDefinition(
			USimpleRPGAttributeSet::GetAD_AttackPowerAttribute(),			//Attribute
			EGameplayEffectAttributeCaptureSource::Source,					//공격자
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

		DamageToMonsterDef = FGameplayEffectAttributeCaptureDefinition(
			USimpleRPGMonsterAttributeSet::GetIncomingDamageAttribute(),
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

namespace 
{
	struct FDamageCalcInput
	{
		//Source Info
		float AttackPower = 0.0f;
		float CriticalChance = 0.0f;
		float CriticalMultiplier = 1.0f;
		float SkillRatio = 0.0f;			//SetByCaller로 읽음 (Non-Attribute)

		//Target Info
		float Defense = 0.0f;				//Target Defense
		float FinalDamage = 0.0f;			//Damage To Target
	};

	FDamageCalcInput CaptureAttributes(const FGameplayEffectCustomExecutionParameters& ExecutionParams)
	{
		const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

		//Result Struct
		FDamageCalcInput Result;

		//Lamda, Attempt를 함수화
		auto Capture = [&](const FGameplayEffectAttributeCaptureDefinition& Def, float& Out, const TCHAR* Name)
			{
				if (!ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
					Def, FAggregatorEvaluateParameters(), Out))
				{
					UE_LOG(LogTemp, Warning, TEXT("Capture Failed : %s"), Name);
				}
			};

		//Capture_Source
		Capture(DamageStatics().AD_AttackPowerDef,		 Result.AttackPower,			TEXT("AD_AttackPower"));
		Capture(DamageStatics().CriticalChanceDef,		 Result.CriticalChance,			TEXT("CriticalChance"));
		Capture(DamageStatics().CriticalMultiplierDef,	 Result.CriticalMultiplier,		TEXT("CriticalMultiplier"));

		//Capture_Target
		Capture(DamageStatics().AD_DefenseDef,			 Result.Defense,				TEXT("AD_Defense"));

		//Get SkillRatio from SetByCaller
		Result.SkillRatio = Spec.GetSetByCallerMagnitude(
			SimpleRPGGameplayTags::SetByCaller_Skill_Ratio,
			true,    // bWarnIfNotFound = true → 태그 못 찾으면 엔진이 알아서 경고
			0.0f);

		UE_LOG(LogTemp, Warning, TEXT("Critical! %f"), Result.CriticalChance);

		return Result;
	}

	float CalculateBaseDamage(const FDamageCalcInput& In)
	{
		const float RandomMin = 0.9f;
		const float RandomMax = 1.1f;
		float Random = FMath::RandRange(RandomMin, RandomMax);
		
		float Base = In.AttackPower * Random * In.SkillRatio;
		
		return Base;
	}

	float ApplyCritical(float BaseDamage, const FDamageCalcInput& In)
	{
		float CriticalDamage = BaseDamage * (FMath::FRand() < In.CriticalChance ? In.CriticalMultiplier : 1.0);

		return CriticalDamage;
	}

	float ApplyDefense(float Damage, const FDamageCalcInput& In)
	{
		//Final = crit * (1 - Defense / Defense + K 방어상수)
		const float K = 100.0f;	//TODO : 밸런싱 시 전역 데이터로 이동
		const float Denom = In.Defense + K;
		if (Denom <= 0.0f) return Damage;
		
		float Final = Damage * (1 - In.Defense / Denom);
		Final = FMath::Max(0.0f, Final);

		return Final;
	}
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

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FDamageCalcInput In = CaptureAttributes(ExecutionParams);
	float Damage = CalculateBaseDamage(In);
	Damage = ApplyCritical(Damage, In);
	Damage = ApplyDefense(Damage, In);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			DamageStatics().DamageToMonsterDef.AttributeToCapture,	//어디에
			EGameplayModOp::Additive,							//더하기
			Damage											//얼마를
		)
	);

	UE_LOG(LogTemp, Warning, TEXT("Final Damage Output : %.1f"), Damage);
}
