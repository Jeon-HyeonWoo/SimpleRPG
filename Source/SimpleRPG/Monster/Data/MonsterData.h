// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MonsterData.generated.h"

class UGameplayEffect;

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
};
