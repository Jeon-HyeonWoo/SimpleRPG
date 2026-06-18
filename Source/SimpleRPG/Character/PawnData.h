// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SimpleRPG/Character/Data/PlayerStat.h"
#include "PawnData.generated.h"

/**
 * 
 */
class USimpleRPGAbilitySet;
class USimpleRPGInputConfig;
class UWeaponData;

UCLASS(BlueprintType, Const, Meta = (DisplayName = "SimpleRPG Pawn Data"))
class SIMPLERPG_API UPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Pawn")
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Abilities")
	TObjectPtr<USimpleRPGAbilitySet> AbilitySet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Input")
	TObjectPtr<USimpleRPGInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Weapon")
	TObjectPtr<UWeaponData> DefaultWeaponData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Weapon")
	TArray<TObjectPtr<UWeaponData>> WeaponSlots;
	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Stat")
	FPlayerStat DefaultStats;

	/*
	* TODO
	* AbilitySet //DO
	* InputConfig
	* DefaultUISet
	*/
};
