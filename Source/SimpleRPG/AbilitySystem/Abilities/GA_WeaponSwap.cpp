// Fill out your copyright notice in the Description page of Project Settings.


#include "GA_WeaponSwap.h"
#include "SimpleRPG/SimpleRPGGameplayTag.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "../../Equipment/SimpleRPGEquipmentComponent.h"
#include "../../Weapon/WeaponData.h"
#include "AbilitySystemComponent.h"

UGA_WeaponSwap::UGA_WeaponSwap()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	// Acting 계열 상호 차단
	ActivationOwnedTags.AddTag(SimpleRPGGameplayTags::State_Acting_Swapping);
	ActivationBlockedTags.AddTag(SimpleRPGGameplayTags::State_Acting);
	ActivationBlockedTags.AddTag(SimpleRPGGameplayTags::State_Dashing);
}

void UGA_WeaponSwap::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* Actorinfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, Actorinfo, ActivationInfo))
	{
		EndAbility(Handle, Actorinfo, ActivationInfo, true, true);
		return;
	}

	AActor* AvatarActor = GetAvatarActorFromActorInfo();

	if (!AvatarActor)
	{
		EndAbility(Handle, Actorinfo, ActivationInfo, true, true);
		return;
	}

	EquipmentComponent = AvatarActor->FindComponentByClass<USimpleRPGEquipmentComponent>();
	if (!EquipmentComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs EquipmentComponent is null"), __LINE__, __FUNCTION__);
		return;
	}

	PendingWeaponData = EquipmentComponent->GetPendingWeaponData();

	if (!PendingWeaponData)
	{
		UE_LOG(LogTemp, Error, TEXT("%d, %hs, PendingWeaponData is null"), __LINE__, __FUNCTION__);
		return;
	}

	//이동 제한 적용
	if (BlockMovementEffect)
	{
		BlockMovementEffectHandle = ApplyGameplayEffectToOwner(
			Handle, Actorinfo, ActivationInfo, BlockMovementEffect.GetDefaultObject(), 1.0f);
	}

	PlaySheathMontage();


}

void UGA_WeaponSwap::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* Actorinfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (BlockMovementEffectHandle.IsValid())
	{
		GetAbilitySystemComponentFromActorInfo()->RemoveActiveGameplayEffect(BlockMovementEffectHandle);
	}

	Super::EndAbility(Handle, Actorinfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGA_WeaponSwap::PlaySheathMontage()
{
	const UWeaponData* CurrentWeaponData = EquipmentComponent->GetCurrentWeaponData();
	UAnimMontage* sheathMontage = CurrentWeaponData ? CurrentWeaponData->SheathMontage.Get() : nullptr;

	if (!sheathMontage)
	{
		OnSheathCompleted();
		return;
	}

	UAbilityTask_PlayMontageAndWait* SheathTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		sheathMontage
	);
	SheathTask->OnCompleted.AddDynamic(this, &UGA_WeaponSwap::OnSheathCompleted);
	SheathTask->OnBlendOut.AddDynamic(this, &UGA_WeaponSwap::OnSheathCompleted);
	SheathTask->OnCancelled.AddDynamic(this, &UGA_WeaponSwap::OnSheathCencelled);
	SheathTask->OnInterrupted.AddDynamic(this, &UGA_WeaponSwap::OnSheathCencelled);
	SheathTask->ReadyForActivation();
}

void UGA_WeaponSwap::PlayDrawMontage()
{
	UAnimMontage* DrawMontage = PendingWeaponData ? PendingWeaponData->DrawMontage.Get() : nullptr;
	if (!DrawMontage)
	{
		FinishSwap();
		return;
	}

	UAbilityTask_PlayMontageAndWait* DrawTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this,
		NAME_None,
		DrawMontage
	);

	DrawTask->OnCompleted.AddDynamic(this, &UGA_WeaponSwap::OnDrawCompleted);
	DrawTask->OnBlendOut.AddDynamic(this, &UGA_WeaponSwap::OnDrawCompleted);
	DrawTask->OnCancelled.AddDynamic(this, &UGA_WeaponSwap::OnDrawCencelled);
	DrawTask->OnInterrupted.AddDynamic(this, &UGA_WeaponSwap::OnDrawCencelled);
	DrawTask->ReadyForActivation();
}

void UGA_WeaponSwap::OnSheathCompleted()
{
	EquipmentComponent->EquipWeapon(PendingWeaponData);

	PlayDrawMontage();
}

void UGA_WeaponSwap::OnSheathCencelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_WeaponSwap::OnDrawCompleted()
{ 
	FinishSwap();
}

void UGA_WeaponSwap::OnDrawCencelled()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
}

void UGA_WeaponSwap::FinishSwap()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

