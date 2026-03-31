// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayAbilitySpecHandle.h"
#include "../AbilitySystem/SimpleRPGAbilitySet.h"
#include "SimpleRPGEquipmentComponent.generated.h"

class UWeaponData;
class UAbilitySystemComponent;
class USimpleRPGGameplayAbility;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLERPG_API USimpleRPGEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USimpleRPGEquipmentComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

public:

	/* PawnData의 WeaponSlot을 받아서 초기화 */
	void InitializeWeaponSlot(const TArray<TObjectPtr<UWeaponData>>& InWeaponSlots);

	/* 입력 시 호출, PendingWeaponData 세팅 후 GA_WeaponSwap 활성화*/
	void RequestWeaponSwap(int32 SlotIndex);

	void EquipWeapon(const UWeaponData* NewWeaponData);
	void UnEquipWeapon();

	const UWeaponData* GetCurrentWeaponData() const { return CurrentWeaponData; }
	const UWeaponData* GetPendingWeaponData() const { return PendingWeaponData; }

private:

	UAbilitySystemComponent* GetASC() const;

private:

	UPROPERTY()
	TArray<TObjectPtr<UWeaponData>> WeaponSlots;

	UPROPERTY()
	TObjectPtr<const UWeaponData> CurrentWeaponData;

	UPROPERTY()
	TObjectPtr<const UWeaponData> PendingWeaponData;

	/* RequestWepaonSwap에서 활성화 할 GA class */
	UPROPERTY(EditDefaultsOnly, Category = "SimpleRPG|Ability")
	TSubclassOf<USimpleRPGGameplayAbility> WeaponSwapAbilityClass;


	FSimpleRPGAbilitySet_GrantedHandles GrantedHandles;
		
};
