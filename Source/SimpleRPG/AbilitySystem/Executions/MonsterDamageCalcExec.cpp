// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterDamageCalcExec.h"
#include "SimpleRPG/AbilitySystem/SimpleRPGMonsterAttributeSet.h"
#include "SimpleRPG/AbilitySystem/SimpleRPGAttributeSet.h"

UMonsterDamageCalcExec::UMonsterDamageCalcExec()
{
	//어떤 Attribute를 캡쳐할 것인가?
	DamageCapture.AttributeToCapture = USimpleRPGMonsterAttributeSet::GetDamageAttribute();

	//어디서 캡쳐할 것인가?
	DamageCapture.AttributeSource = EGameplayEffectAttributeCaptureSource::Source;

	//SnapShot 여부 true = GE적용 시점의 값을 고정
	DamageCapture.bSnapshot = true;

	//Capture된 DamageCapture변수를 ExecutionParams에서 꺼내 쓸 수 있도록 등록
	RelevantAttributesToCapture.Add(DamageCapture);

}

void UMonsterDamageCalcExec::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	//GA에서 넘겨받은 GESpec 가져오기.
	const FGameplayEffectSpec& GESpec = ExecutionParams.GetOwningSpec();
	
	//SetByCaller에서 Multiplier 꺼내기
	float Multiplier = GESpec.GetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Data.Damage.Multiplier")),
		true,
		0.0f);

	//Capture된 Damage값 꺼내기
	FAggregatorEvaluateParameters EvalParams;
	float DamageValue = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageCapture, EvalParams, DamageValue);

	//Final Damage 계산
	float FinalDamage = DamageValue * Multiplier;

	//Output으로 내보내기
	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			USimpleRPGAttributeSet::GetHealthAttribute(),
			EGameplayModOp::Additive,
			-FinalDamage
		)
	);
}
