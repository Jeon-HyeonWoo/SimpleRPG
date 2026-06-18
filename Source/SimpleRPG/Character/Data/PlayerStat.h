// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStat.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FPlayerStat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxMP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AD_AttackPower = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AP_AttackPower = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AD_Defense = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AP_Defense = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float CriticalChance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CriticalMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MovementSpeedMultiplier = 1.0f;


};