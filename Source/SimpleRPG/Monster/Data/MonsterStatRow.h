#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MonsterStatRow.generated.h"

USTRUCT(BlueprintType)
struct FMonsterStatRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxHP = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AD_AttackPower = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AP_AttackPower = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AD_Defense = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AP_Defense = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AttackSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin = "0.0", ClampMax="1.0"))
	float CriticalChance = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CriticalMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MoveSpeedMutiplier = 1.0f;

	
};