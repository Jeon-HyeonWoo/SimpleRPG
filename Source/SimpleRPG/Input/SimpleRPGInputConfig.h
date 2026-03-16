// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "SimpleRPGInputConfig.generated.h"

/**
 * 
 */

class UInputAction;

USTRUCT(BlueprintType)
struct FSimpleRPGInputAction
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;
};

UCLASS()
class SIMPLERPG_API USimpleRPGInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Input")
	TArray<FSimpleRPGInputAction> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Input")
	TArray<FSimpleRPGInputAction> AbilityInputActions;
};
