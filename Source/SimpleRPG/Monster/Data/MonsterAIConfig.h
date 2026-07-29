#pragma once

#include "CoreMinimal.h"
#include "MonsterAIConfig.generated.h" 

USTRUCT(BlueprintType)
struct FMonsterAIConfig
{
	GENERATED_BODY()


#pragma region Range

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin="0.0"))
	float DetectionRange = 400.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float AttackRange = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float LeashRange = 800.0f;

	//PartolRagne < LeashRange
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float PatrolRange = 400.0f;

#pragma endregion


#pragma region Speed

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float PatrolSpeed = 150.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float ChaseSpeed = 450.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float ReturnSpeed = 500.0f;

#pragma endregion

};