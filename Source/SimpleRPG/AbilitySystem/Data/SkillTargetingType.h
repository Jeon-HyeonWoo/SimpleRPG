// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillTargetingType.Generated.h"

UENUM(BlueprintType)
enum class ESkillTargettingType : uint8
{
	Immediate		UMETA(DisplayName = "Immediate"),
	TargetLockOn	UMETA(DisplayName = "TargetLockOn"),
	LocationAOE		UMETA(DisplayName = "LocationAOE")

};