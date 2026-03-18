// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleRPGAbilityInputID.generated.h"


UENUM(BlueprintType)
enum class ESimpleRPGAbilityInputID : uint8
{
	None = 0,
	Dash,
	Attack,
	Skill1,
	Skill2,
	Skill3
};