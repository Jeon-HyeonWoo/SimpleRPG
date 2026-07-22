// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "SimpleRPGTeam.generated.h"


UENUM(BlueprintType)
enum class ESimpleRPGTeam : uint8
{
	Player = 0,
	Ally = 1,
	Monster = 2,
	Neutral = 255
};