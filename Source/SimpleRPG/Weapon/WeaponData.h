// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"

/**
 * 
 */

class UAnimInstance;
class UAnimMontage;
class USimpleRPGAbilitySet;


UCLASS()
class SIMPLERPG_API UWeaponData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public: 

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TSubclassOf<UAnimInstance> AnimLayerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> DrawMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> SheathMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Ability")
	TObjectPtr<USimpleRPGAbilitySet> AbilitySet;
	
};
