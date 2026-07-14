// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Engine/DataTable.h"
#include "EMonsterTier.h"
#include "MonsterAIConfig.h"
#include "MonsterActionData.h"
#include "MonsterData.generated.h"

class UGameplayEffect;
class UBehaviorTree;
class USimpleRPGAbilitySet;
/**
 * 
 */
UCLASS()
class SIMPLERPG_API UMonsterData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

#pragma region Identity
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Identity")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Identity")
	EMonsterTier MonsterTier;
#pragma endregion


#pragma region Stat
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Stat",
		meta = (RowType = "/Script/SimpleRPG.MonsterStatRow"))
	FDataTableRowHandle StatRowHandle;
#pragma endregion


#pragma region AI
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Identity")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|AI")
	FMonsterAIConfig AIConfig;
#pragma endregion

#pragma region Action
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Action")
	TMap<FGameplayTag, FMonsterActionData> ActionDataMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Identity")
	TObjectPtr<USimpleRPGAbilitySet> AbilitySet;
#pragma endregion
	
	const FMonsterActionData* FindActionData(const FGameplayTag& ActionTag) const;
};
