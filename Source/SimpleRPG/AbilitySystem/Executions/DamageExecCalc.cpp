// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageExecCalc.h"
#include "../SimpleRPGAttributeSet.h"
#include "../SimpleRPGMonsterAttributeSet.h"

UDamageExecCalc::UDamageExecCalc()
{
	PlayerStatPower = FGameplayEffectAttributeCaptureDefinition(
		USimpleRPGAttributeSet::GetPowerAttribute(),			//Capture Attribute
		EGameplayEffectAttributeCaptureSource::Source,			//From Source(Player)
		false													//None Snap shot (적용 시점 실시간 값)
	);

	TargetMonsterHp = FGameplayEffectAttributeCaptureDefinition(
		USimpleRPGMonsterAttributeSet::GetHPAttribute(),		//Capture Attribute
		EGameplayEffectAttributeCaptureSource::Target,			//From Target(Monster)
		false													//None Snap shot
	);

	/* 
		RelevantAttributesToCapture : 부모 클래스가 가지고 있는 배열로 해당 배열에 등록해야 Execute에서 Attribute값을 읽을 수 있다.
	*/
	RelevantAttributesToCapture.Add(PlayerStatPower);
	RelevantAttributesToCapture.Add(TargetMonsterHp);
}

void UDamageExecCalc::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	if (SourceASC)
	{
		const USimpleRPGAttributeSet* AS = SourceASC->GetSet<USimpleRPGAttributeSet>();
		if (AS)
		{
			UE_LOG(LogTemp, Warning, TEXT("Direct Source Power: %f"), AS->GetPower());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Source has no SimpleRPGAttributeSet"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Source ASC"));
	}

	//1. 캡쳐된 Player Stat Power 값 읽기
	float Power = 0.0f;

	/* 
	* ExecutionParams : 계산에 필요한 모든 정보를 담고 있는 구조체
	* 생성자에서 등록한 캡처 정의를 기반으로 값을 읽어 마지막 파라미터에 전달
	* 평가파라미터는 버프/디버프 같은 모디파이어 평가
	*/

	bool bSuccess = ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		PlayerStatPower,					//생성자에서 정의한 캡처
		FAggregatorEvaluateParameters(),	//평가 파라미터 
		Power								//결과 값을 해당 변수에 저장
	);
	UE_LOG(LogTemp, Warning, TEXT("ExecCalc - Power Capture Success : %s, Power : %f"), bSuccess ? TEXT("true") : TEXT("False"), Power);

	//2. SetByCaller로 Ability에서 넘긴 배율 범위 읽기
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	float MinMultiplier = Spec.GetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Damage.Multiplier.Min")),
		false,
		0.0f
	);

	float MaxMultiplier = Spec.GetSetByCallerMagnitude(
		FGameplayTag::RequestGameplayTag(FName("Damage.Multiplier.Max")),
		false,
		0.0f
	);

	UE_LOG(LogTemp, Warning, TEXT("ExecCalc - Min: %f, Max: %f"), MinMultiplier, MaxMultiplier);

	//3. 랜덤 데미지 계산
	float Multiplier = FMath::RandRange(MinMultiplier, MaxMultiplier);
	float FinalDamage = FMath::TruncToFloat(Power * Multiplier);

	UE_LOG(LogTemp, Warning, TEXT("Execalc - finalDamage %f"), FinalDamage);

	//4. 결과 출력
	/* 
	* OutExecutionOutput : 계산 결과를 시스템에 전달하는 출력 통로
	* AddOutputModifier : 어떤 Attribute를, 어떠한 연산으로, 얼마만큼 변경할 것인지
	*/
	OutExecutionOutput.AddOutputModifier(
		FGameplayModifierEvaluatedData(
			USimpleRPGMonsterAttributeSet::GetHPAttribute(),
			EGameplayModOp::Additive,
			-FinalDamage
		)
	);
}
