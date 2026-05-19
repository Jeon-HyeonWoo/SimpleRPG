#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MonsterDamageTableRow.generated.h"

USTRUCT(BlueprintType)
struct FMonsterDamageDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MinMultiplier = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MaxMultiplier = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CoolDown = 0.0f;

};