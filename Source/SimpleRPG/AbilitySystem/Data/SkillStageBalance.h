// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillStageBalance.Generated.h"

/*
* 스킬 한 단계 데미지 수치
* - 콤보 : "Combo1"...
* - 차지 : "UnderCharge", "FullCharge", "OverCharge"
* - 단계가 없는 단타 스킬도 항목 1개는 가지도록 
*/
USTRUCT(BlueprintType)
struct FSkillStageBalance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Stage")
	FName StageId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimplePPG|Stage", meta = (ClampMin = "0.0"))
	float DamageMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimplePPG|Stage", meta = (ClampMin = "0.0"))
	float DamageMultiplierPerGrade = 0.0f;
};