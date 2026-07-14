// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "MonsterActionData.generated.h"


USTRUCT(BlueprintType)
struct FMonsterMontageEntry
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> Montage;

	UPROPERTY(EditDefaultsOnly, meta = (RowType = "/Script/SimpleRPG.MonsterDamageDataTableRow"))
	FDataTableRowHandle DamageRowHandle;
	
};

USTRUCT(BlueprintType)
struct FMonsterActionData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FMonsterMontageEntry> MontageEntries;

	const FMonsterMontageEntry* GetRandomEntry() const;
};