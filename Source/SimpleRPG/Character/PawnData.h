// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PawnData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "SimpleRPG Pawn Data"))
class SIMPLERPG_API UPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SimpleRPG|Pawn")
	TSubclassOf<APawn> PawnClass;

	/*
	* TODO
	* AbilitySet
	* InputConfig
	* DefaultUISet
	*/
};
