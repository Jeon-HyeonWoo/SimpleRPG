// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "MonsterDamageCalcExec.generated.h"

/**
 * - ApplyGameplayEffectSpecToTarget등으로 GameplayEffect가 호출이 될 때,
 * GameplayEffect에 ExectuionCalculation이 세팅이 되어있다면,
 * Execute_Implementaion이 자동으로 호출된다.
 * 해당 계산 결과물은 Target Attribute에 반영한다.
 * 
 * - ExecutionParams의 정보에는
 * Caputured Attribute Value(생성자에서 등록한 것들)
 * GameplayEffectSpec (SetByCaller, Context 값)
 * Source/Target ASC, Tag정보 등등
 * 
 * - OutExecutionOutput(출력) Attribute를 변경하라는 계산 결과를 엔진에 돌려줌.
 */

/*
* 내부 단계
* 1. GESpec 가져오기 
* - GA에서 MakeOutgoingGameplayEffectSpec으로 만든 Spec이 전달.
* - SetByCaller로 넣은 값도 포함
* 
* 2. SetByCaller값 꺼내기
* Spec.GetSetByCallerMagnitude(Tag, bWarnIfNotFound, DefaultValue) 
* SetSetByCaller로 넣은 값을 꺼내는 것. 
* 
* 3. Capture된 Attribute값 꺼내기
* 
* 4. 계산결과 output으로 내보내기
*/
UCLASS()
class SIMPLERPG_API UMonsterDamageCalcExec : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:

	UMonsterDamageCalcExec();

	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput
	) const override;

private:

	//Source의 MonsterAttributeSet::Damage 캡쳐
	FGameplayEffectAttributeCaptureDefinition DamageCapture;
};
