// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "MonsterAnimSet.generated.h"

/**
 * 
 */

USTRUCT()
struct FMonsterAnimList
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TObjectPtr<UAnimMontage>> AnimList;

};

USTRUCT(BlueprintType)
struct FMonsterAnimSet
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, FMonsterAnimList> AnimSet;

	UAnimMontage* GetAnimMontageByTag(FGameplayTag Tag) const;
};
