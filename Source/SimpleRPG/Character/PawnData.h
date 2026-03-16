// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PawnData.generated.h"

/**
 * 
 */
class USimpleRPGAbilitySet;

UCLASS(BlueprintType, Const, Meta = (DisplayName = "SimpleRPG Pawn Data"))
class SIMPLERPG_API UPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Pawn")
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Abilities")
	TObjectPtr<USimpleRPGAbilitySet> AbilitySet;

	/*
	* TODO
	* AbilitySet
	* InputConfig
	* DefaultUISet
	*/
};
