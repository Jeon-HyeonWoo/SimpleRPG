// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGEquipmentComponent.h"
#include "../Weapon/WeaponData.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "../AbilitySystem/SimpleRPGAbilitySet.h"
#include "../AbilitySystem/Abilities/SimpleRPGGameplayAbility.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerState.h"

// Sets default values for this component's properties

USimpleRPGEquipmentComponent::USimpleRPGEquipmentComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USimpleRPGEquipmentComponent::EquipWeapon(const UWeaponData* NewWeaponData)
{
	if (!IsValid(NewWeaponData))
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, NewWeaponData is invalid"), __LINE__, __FUNCTION__);
		return;
	}

	if (CurrentWeaponData == NewWeaponData)
	{
		return;
	}

	/* 1. 기존 무기 해제 */
	UnEquipWeapon();

	UAbilitySystemComponent* ASC = GetASC();

	if (!IsValid(ASC))
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, ASC is invalid"), __LINE__, __FUNCTION__);
		return;
	}

	/* 2. 새로운 무기 어빌리티 부여 */
	if (IsValid(NewWeaponData->AbilitySet))
	{
		NewWeaponData->AbilitySet->GiveToAbilitySystem(ASC, &GrantedHandles);
	}

	/* 3. 새로운 무기의 AnimLayer 적용 */
	if (NewWeaponData->AnimLayerClass)
	{
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
		{
			if (UAnimInstance* AnimInst = OwnerCharacter->GetMesh()->GetAnimInstance())
			{
				AnimInst->LinkAnimClassLayers(NewWeaponData->AnimLayerClass);
			}
		}
	}

	/* 4. 현재 Weapon을 새로운 Weapon Data로 변경 */
	CurrentWeaponData = NewWeaponData;
}

void USimpleRPGEquipmentComponent::UnEquipWeapon()
{
	if (!CurrentWeaponData)
	{
		return;
	}

	UAbilitySystemComponent* ASC = GetASC();

	if (!IsValid(ASC))
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, ASC is invalid"), __LINE__, __FUNCTION__);
		return;
	}

	/* 1. 기존 Ability 제거 */
	GrantedHandles.RemoveFromAbilitySystem(ASC);

	/* 2. 기존 AnimLayer 해제 */
	if (CurrentWeaponData->AnimLayerClass)
	{
		if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
		{
			if (UAnimInstance* AnimInst = OwnerCharacter->GetMesh()->GetAnimInstance())
			{
				AnimInst->UnlinkAnimClassLayers(CurrentWeaponData->AnimLayerClass);
			}
		}
	}

	CurrentWeaponData = nullptr;
}

UAbilitySystemComponent* USimpleRPGEquipmentComponent::GetASC() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if (!IsValid(OwnerPawn))
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, OwnerPawn is invalid"), __LINE__, __FUNCTION__);
		return nullptr;
	}

	AController* Controller = OwnerPawn->GetController();

	if (!IsValid(Controller))
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, Controller is invalid"), __LINE__, __FUNCTION__);
		return nullptr;
	}

	APlayerState* PS = Controller->GetPlayerState<APlayerState>();

	if (!IsValid(PS))
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, PlayerState is invalid"), __LINE__, __FUNCTION__);
		return nullptr;
	}


	return PS->FindComponentByClass<UAbilitySystemComponent>();
}
