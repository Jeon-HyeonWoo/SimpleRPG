#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "DamageDataTableRow.generated.h"

USTRUCT(BlueprintType)
struct FDamageDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SkillRatio = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CoolDown = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ManaCost = 0.0f;
};