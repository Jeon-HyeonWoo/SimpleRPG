// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "DamageExecCalc.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLERPG_API UDamageExecCalc : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:

	UDamageExecCalc();

	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,	//계산에 필요한 정보 묶음
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput			
	) const override;
	
private:

	FGameplayEffectAttributeCaptureDefinition PlayerStatPower;
	FGameplayEffectAttributeCaptureDefinition TargetMonsterHp;
};
