// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleRPGEquipmentComponent.h"
#include "../Weapon/WeaponData.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "../AbilitySystem/SimpleRPGAbilitySet.h"
#include "../AbilitySystem/Abilities/SimpleRPGGameplayAbility.h"
#include "GameFramework/Controller.h"
#include "../Player/SimpleRPGPlayerState.h"
#include "Components/SkeletalMeshComponent.h"
#include "../Character/SimpleRPGPlayerCharacter.h"

// Sets default values for this component's properties

USimpleRPGEquipmentComponent::USimpleRPGEquipmentComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USimpleRPGEquipmentComponent::InitializeWeaponSlot(const TArray<TObjectPtr<UWeaponData>>& InWeaponSlots)
{
	WeaponSlots = InWeaponSlots;
}

void USimpleRPGEquipmentComponent::RequestWeaponSwap(int32 SlotIndex)
{
	//slot index 유효성 검사
	if (!WeaponSlots.IsValidIndex(SlotIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, invalid slot index"), __LINE__, __FUNCTION__);
		return;
	}

	//Swap 대상 Weapon Data
	const UWeaponData* TargetWeaponData = WeaponSlots[SlotIndex];

	if (!IsValid(TargetWeaponData))
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, TargetWaeponData is invalid"), __LINE__, __FUNCTION__);
		return;
	}

	//현재 무기와 다음 교체 무기가 같다면 무시
	if (CurrentWeaponData == TargetWeaponData)
	{
		return;
	}

	UAbilitySystemComponent* ASC = GetASC();
	if (!ASC)
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, ASC is invalid"), __LINE__, __FUNCTION__);
		return;
	}

	//GA_WeaponSwap 이 읽어갈 변수값 세팅
	PendingWeaponData = TargetWeaponData;

	if (WeaponSwapAbilityClass)
	{
		/* 활성화 실패시 PendingData 정리 */
		if (!ASC->TryActivateAbilityByClass(WeaponSwapAbilityClass))
		{
			PendingWeaponData = nullptr;
			UE_LOG(LogTemp, Error, TEXT("%d, %hs, Faild to activate Weapon Swap Ability"), __LINE__, __FUNCTION__);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, WeaponSwapAbilityClass is not Set"), __LINE__, __FUNCTION__);
	}

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

	//GA에서 사용된 PendingData 정리
	PendingWeaponData = nullptr;
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
	ASimpleRPGPlayerCharacter* OwnerCharacter = Cast<ASimpleRPGPlayerCharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		return nullptr;
	}

	return OwnerCharacter->GetAbilitySystemComponent();
}
