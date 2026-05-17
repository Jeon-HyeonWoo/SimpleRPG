// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EMonsterTier.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Data")
	TSubclassOf<UGameplayEffect> InitStatsEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Data")
	EMonsterTier MonsterTier;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Data")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Data")
	TObjectPtr<UBehaviorTree> BehaviorTree;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Data")
	TObjectPtr<USimpleRPGAbilitySet> AbilitySet;

};
