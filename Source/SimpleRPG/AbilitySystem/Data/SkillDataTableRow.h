// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "SkillDataTableRow.generated.h"

USTRUCT(BlueprintType)
struct FSkillDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

#pragma region Identity

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Identity")
	FName SkillCode = NAME_None;

#pragma endregion


#pragma region Growth

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Growth", meta = (ClampMin = "1", ClampMax = "10"))
	int32 MaxGrade = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Growth", meta = (ClampMin = "0.0", ClampMax = "100.0"))
	float MaxAbility = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Growth", meta = (ClampMin = "0.0"))
	float AbilityGainOnUse = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Growth", meta = (ClampMin = "0.0"))
	float AbilityGainOnKill = 0.0f;
	
#pragma endregion


#pragma region Cost

	/* 소모 자원 종류 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Cost", meta = (Categories = "Skill.Cost"))
	FGameplayTag CostType;

	/* Grade Level 1의 Cost Value */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Cost", meta = (ClampMin = "0.0"))
	float BaseCostValue = 0.0f;

	/* Grade당 자원 소모 추가량 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Cost", meta = (ClampMin = "0.0"))
	float CostPerGrade = 0.0f;

	/* 재사용 대기시간(초) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Cost", meta = (ClampMin = "0.0"))
	float CooldownDuration = 0.0f;

#pragma endregion


#pragma region Damage

	/* 기본 공격력 대비 공격력 계수 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Damage", meta = (ClampMin = "0.0"))
	float DamageMultiplier = 1.0f;

	/* Grade 대비 추가되는 계수 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Damage", meta = (ClampMin = "0.0"))
	float DamageMultiplierPerGrade = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Damage", meta = (Categories = "Damage.Type"))
	FGameplayTag DamageType;

#pragma endregion


#pragma region Targeting

	/* 최대 타격 대상 수. 0 = 대상수 제한 없음 or 비공격형 스킬 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Targeting", meta = (ClampMin = "0"))
	int32 MaxTargets = 1;

	/* 유효 사거리 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SimpleRPG|Targeting", meta = (ClampMin = "0.0"))
	float Range = 0.0f;
	 

#pragma endregion

#pragma region Helper

	/* 등급 반영한 최종 공격력 계수 반환 */
	float GetDamageMultifilerAtGrade(int32 InGrade) const;

	/* 등급을 반영한 최종 자원 소모량 반환 */
	float GetCostAtGrade(int32 InGrade) const;


private:

	/* 외부에서 들어온 Grade를 범위 조정 */
	int GetClampedGrade(int32 InGrade) const;
#pragma endregion
};
