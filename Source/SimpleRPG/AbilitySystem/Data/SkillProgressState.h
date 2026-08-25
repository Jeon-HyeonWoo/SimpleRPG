// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillProgressState.Generated.h"

/*
* SkillDataTableRow와 분리 하여 DT를 수정해도 값이 변하지 않도록
*/
USTRUCT(BlueprintType)
struct FSkillProgressState
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category = "SimpleRPG|Progress")
	FName SkillCode = NAME_None;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category = "SimpleRPG|Progress")
	int32 CurrentGrade = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, SaveGame, Category = "SimpleRPG|Progress")
	float CurrentAbility = 0.0f;
};