// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterDamageCalcExec.h"
#include "SimpleRPG/Monster/AbilitySystem/AttributeSet/SimpleRPGMonsterAttributeSet.h"
#include "SimpleRPG/SimpleRPGGameplayTag.h"
#include "SimpleRPG/AbilitySystem/SimpleRPGAttributeSet.h"

struct FMonsterDamageStatics
{
	FGameplayEffectAttributeCaptureDefinition Monster_AD_AttackPowerDef;
	FGameplayEffectAttributeCaptureDefinition Monster_CriticalChanceDef;
	FGameplayEffectAttributeCaptureDefinition Monster_CriticalMultiplierDef;

	FGameplayEffectAttributeCaptureDefinition Player_AD_DefenseDef;
	FGameplayEffectAttributeCaptureDefinition Player_IncomingDamageDef;
	

	FMonsterDamageStatics()
	{
		Monster_AD_AttackPowerDef = FGameplayEffectAttributeCaptureDefinition(
			USimpleRPGMonsterAttributeSet::GetAD_AttackPowerAttribute(),
			EGameplayEffectAttributeCaptureSource::Source,
			false
		);

		Monster_CriticalChanceDef = FGameplayEffectAttributeCaptureDefinition(
			USimpleRPGMonsterAttributeSet::GetCriticalChanceAttribute(),
			EGameplayEffectAttributeCaptureSource::Source,
			false
		);

		Monster_CriticalMultiplierDef = FGameplayEffectAttributeCaptureDefinition(
			USimpleRPGMonsterAttributeSet::GetCriticalMultiplierAttribute(),
			EGameplayEffectAttributeCaptureSource::Source,
			false
		);


		Player_AD_DefenseDef = FGameplayEffectAttributeCaptureDefinition(
			USimpleRPGAttributeSet::GetAD_DefenseAttribute(),
			EGameplayEffectAttributeCaptureSource::Target,
			false
		);

		Player_IncomingDamageDef = FGameplayEffectAttributeCaptureDefinition(
			USimpleRPGAttributeSet::GetIncomingDamageAttribute(),
			EGameplayEffectAttributeCaptureSource::Target,
			false
		);
	}
};

static const FMonsterDamageStatics& MonsterDamageStatics()
{
	static FMonsterDamageStatics MDStatics;
	return MDStatics;
}

namespace
{
	struct FMonsterDamageCalcInput
	{
		float MonsterAttackPower = 0.0f;
		float MonsterCriticalChance = 0.0f;
		float MonsterCriticalMultiplier = 1.0f;
		float MonsterDamageRatio = 0.0f;

		float PlayerADDefense = 0.0f;
		float DamageToPlayerFinalDamage = 0.0f;
	};

	FMonsterDamageCalcInput CaptureMonsterAttributes(const FGameplayEffectCustomExecutionParameters& ExecutionParams)
	{
		const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

		FMonsterDamageCalcInput Result;

		auto Capture = [&](const FGameplayEffectAttributeCaptureDefinition& Def, float& Out, const TCHAR* Name)
			{
				if (!ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(Def, FAggregatorEvaluateParameters(), Out))
				{
					UE_LOG(LogTemp, Warning, TEXT("Capture Failed : %s"), Name);
				}
			};

		Capture(MonsterDamageStatics().Monster_AD_AttackPowerDef, Result.MonsterAttackPower, TEXT("Monster_AD_AttackPower"));
		Capture(MonsterDamageStatics().Monster_CriticalChanceDef, Result.MonsterCriticalChance, TEXT("Monster_CriticalChance"));
		Capture(MonsterDamageStatics().Monster_CriticalMultiplierDef, Result.MonsterCriticalMultiplier, TEXT("Monster_CriticalMultiplier"));

		Capture(MonsterDamageStatics().Player_AD_DefenseDef, Result.PlayerADDefense, TEXT("Player_AD_Defense"));

		Result.MonsterDamageRatio = Spec.GetSetByCallerMagnitude(
			SimpleRPGGameplayTags::SetByCaller_Skill_Ratio,
			true,
			0.0f
		);


		return Result;
	}

	//Base Damage에 랜덤 데미지 부여
	float CalculateBaseDamage(const FMonsterDamageCalcInput& In)
	{
		const float RandomMin = 0.9f;
		const float RandomMax = 1.1f;
		float Random = FMath::RandRange(RandomMin, RandomMax);

		float Base = In.MonsterAttackPower * Random * In.MonsterDamageRatio;

		return Base;
	}

	float ApplyCritical(float Damage, const FMonsterDamageCalcInput& In)
	{
		float CriticalDamage = Damage * (FMath::FRand() < In.MonsterCriticalChance ? In.MonsterCriticalMultiplier : 1.0f);

		return CriticalDamage;
	}

	//방어력 계수
	float ApplyDefense(float Damage, const FMonsterDamageCalcInput& In)
	{
		const float K = 100.0f;
		const float Denom = In.PlayerADDefense + K;
		if (Denom <= 0.0f) return Damage;

		float Final = Damage * (1 - In.PlayerADDefense / Denom);
		Final = FMath::Max(0.0f, Final);
		
		return Final;
	}
}

UMonsterDamageCalcExec::UMonsterDamageCalcExec()
{
	RelevantAttributesToCapture.Add(MonsterDamageStatics().Monster_AD_AttackPowerDef);
	RelevantAttributesToCapture.Add(MonsterDamageStatics().Monster_CriticalChanceDef);
	RelevantAttributesToCapture.Add(MonsterDamageStatics().Monster_CriticalMultiplierDef);
	
	RelevantAttributesToCapture.Add(MonsterDamageStatics().Player_AD_DefenseDef);
}

void UMonsterDamageCalcExec::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FMonsterDamageCalcInput In = CaptureMonsterAttributes(ExecutionParams);
	float Damage = CalculateBaseDamage(In);
	Damage = ApplyCritical(Damage, In);
	Damage = ApplyDefense(Damage, In);

	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			MonsterDamageStatics().Player_IncomingDamageDef.AttributeToCapture,
			EGameplayModOp::Additive,
			Damage
		)
	);
	
}
