// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "SimpleRPGAbilityInputID.h"
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

	UPROPERTY(EditDefaultsOnly)
	ESimpleRPGAbilityInputID AbilityInputID = ESimpleRPGAbilityInputID::None;
};

UCLASS()
class SIMPLERPG_API USimpleRPGInputConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "SimpleRPG|Input")
	const UInputAction* FindNativeInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UFUNCTION(BlueprintCallable, Category = "SimpleRPG|Input")
	const UInputAction* FindAbilityInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound = true) const;

	UFUNCTION(BlueprintCallable, Category = "SimpleRPG|Input")
	ESimpleRPGAbilityInputID FindAbilityInputIDByTag(const FGameplayTag& InputTag) const;

public:

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Input")
	TArray<FSimpleRPGInputAction> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Input")
	TArray<FSimpleRPGInputAction> AbilityInputActions;
};
