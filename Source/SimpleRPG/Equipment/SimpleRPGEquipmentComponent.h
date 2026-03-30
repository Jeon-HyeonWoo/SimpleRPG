// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayAbilitySpecHandle.h"
#include "../AbilitySystem/SimpleRPGAbilitySet.h"
#include "SimpleRPGEquipmentComponent.generated.h"

class UWeaponData;
class UAbilitySystemComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLERPG_API USimpleRPGEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USimpleRPGEquipmentComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:

	void EquipWeapon(const UWeaponData* NewWeaponData);
	void UnEquipWeapon();

	const UWeaponData* GetCurrentWeaponData() const { return CurrentWeaponData; }

private:

	UAbilitySystemComponent* GetASC() const;

private:

	UPROPERTY()
	TObjectPtr<const UWeaponData> CurrentWeaponData;

	FSimpleRPGAbilitySet_GrantedHandles GrantedHandles;
		
};
