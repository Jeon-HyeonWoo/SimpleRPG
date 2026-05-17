// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EMonsterTier.generated.h"


UENUM(BlueprintType)
enum class EMonsterTier : uint8
{
	Normal,
	Named,
	Boss
};